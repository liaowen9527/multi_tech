
#sample relationship
class RelationShip:

    def __init__(self, _id, parent, _data):
        self._id = _id
        self.parent = parent
        self._data = _data

class NbTreeNode:

    def __init__(self, _id, _data = None):
        self._id = _id
        self._data = _data
        self.children = []
        self.parent = None

class NbTree:

    def __init__(self):
        self.dic_node = {}
        self.nodes = set()
        self.root = NbTreeNode('')


    def build(self, relationship_list):
        for node in relationship_list:
            #make sure has parent noe
            if not self.dic_node.get(node.parent):
                self.dic_node[node.parent] = NbTreeNode(node.parent)

            parent = self.dic_node[node.parent]
            #make sure has item
            if not self.dic_node.get(node._id):
                self.dic_node[node._id] = NbTreeNode(node._id)

            item = self.dic_node[node._id]
            item._data = node._data

            #set parent
            if item == parent:
                self.root.children.append(item)
                item.parent = self.root
            else:
                parent.children.append(item)
                item.parent = parent
            
    def get_tree_node(self, _id):
        return self.dic_node.get(_id)

    def level_loop(self, parent, func):
        if parent is None:
            return

        func(parent)
        for node in parent.children:
            self.level_loop(node, func)

        return ret

    def level_loop(self, parent, func):
        if parent is None:
            return

        func(parent)
        for node in parent.children:
            self.level_loop(node, func)

        return ret

    def insert_item(self, item, parent_id):
        if item._id in self.dic_node:
            return False

        parent = None
        if item._id == parent_id:
            parent = self.root
        else:
            if parent_id not in self.dic_node:
                return False

            parent = self.dic_node[parent_id]

        self.dic_node[item._id] = item
        self.nodes.add(item)

        parent.children.append(item)
        item.parent = parent

        return True

    def delete_item(self, id_):
        if id_ in self.dic_node:
            return

        item = self.dic_node[id_]

        self.dic_node.remove(id_)
        if item.parent is not None:
            item.parent.children.remove(item)

        self.nodes.remove(item)

        return True

    def has_item(self, item):
        return item in self.nodes

    def find_by_id(self, id):
        return self.dic_node.get(_id)


class NbTreePreOrder:
    class Status:
        def __init__(self, node):
            self.node = node
            self.next_child_index = 0

    def __init__(self, node):
        self.status = []
        self.root = node
        self.curr = None

    def __iter__(self):
        return self

    def __next__(self):
        if self.root is None:
            raise StopIteration()

        if self.curr is None:
            self.curr = self.root
            return self.curr

        if len(self.curr.children) == 0:
            if len(self.status) == 0:
                raise StopIteration()
            
            while len(self.status) > 0:
                last = self.status[-1]
                if last.next_child_index >= len(last.node.children):
                    self.status.pop()
                else:
                    self.curr = last.node.children[last.next_child_index]
                    last.next_child_index += 1
                    return self.curr

            raise StopIteration()

        last = NbTreePreOrder.Status(self.curr)
        self.status.append(last)
        self.curr = last.node.children[last.next_child_index]
        last.next_child_index += 1

        return self.curr
