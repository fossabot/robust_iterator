/*
MIT License

Copyright (c) 2018 Dmitry Vodopyanov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef GROUP_H_
#define GROUP_H_

#include <vector>
#include <algorithm>

template <typename T> class CompositeIterator;
template <typename T> class RobustIterator;
template <typename T> class GroupIterator;
template <typename T> class List;

template <typename T> class Group : public Component<T> {
private:
    List<Component<T>>* components = new List<Component<T>>();
    vector<RobustIterator<T>*> iterators;

public:
    void add(Component<T>* item) {
        components->add(item);
    }

    void remove(Component<T>* item) {
        for (int i = 0; i < iterators.size(); i++)
            iterators[i]->notify_remove(item);
        components->remove(item);
    }

    List<Component<T>>* get() {
        return components;
    }

    Iterator<T>* create_iterator() {
        RobustIterator<T>* iterator = new GroupIterator<T>(this);
        iterators.push_back(iterator);

        RobustIterator<T>* composite_iterator = new CompositeIterator<T>(iterator);
        iterators.push_back(composite_iterator);

        return composite_iterator;
    }

    virtual bool is_composite() {
        return true;
    }

    virtual string to_string() {
        string str = "";
        Iterator<T>* iterator = create_iterator();
        while (!iterator->is_done()) {
            Component<int>* item = iterator->next();
            str += item->to_string();

            if (!iterator->is_done())
                str += " ";
        }
        return str;
    }

    void unsubscribe(RobustIterator<T>* iterator) {
        typename vector<RobustIterator<T>*>::iterator it = std::find(iterators.begin(), iterators.end(), iterator);
        if (it != iterators.end())
            iterators.erase(it);
    }
};

#endif  // GROUP_H_
