
#pragma once

#include "lw_util.h"
#include <memory>
#include <list>
#include <map>

namespace lw_util {

	template<class TreeItemPtr>
	class Tree
	{
	public:
		Tree() {}
		~Tree() {}

	public:
		int GetCount()
		{
			return m_items.size();
		}

		TreeItemPtr GetRootItem()
		{
			return GetChildItem(nullptr);
		}

		const std::list<TreeItemPtr>& GetChildren(TreeItemPtr item)
		{
			static std::list<TreeItemPtr> s_children;

			auto itr = m_parents.find(item);
			if (m_parents.end() == itr)
			{
				return s_children;
			}

			return itr->second;
		}

		const std::list<TreeItemPtr>& GetSiblings(TreeItemPtr item)
		{
			TreeItemPtr parent = GetParentItem(item);

			return GetChildren(parent);
		}

		TreeItemPtr GetPrevSiblingItem(TreeItemPtr item)
		{
			const auto& siblings = GetSiblings(item);
			if (siblings.empty())
			{
				return nullptr;
			}

			auto itr = std::find(siblings.begin(), siblings.end(), item);
			if (siblings.end() == itr || siblings.begin() == itr)
			{
				return nullptr;
			}

			itr--;
			return *itr;
		}

		TreeItemPtr GetNextSiblingItem(TreeItemPtr item)
		{
			const auto& siblings = GetSiblings(item);
			if (siblings.empty())
			{
				return nullptr;
			}

			auto itr = std::find(siblings.begin(), siblings.end(), item);
			if (siblings.end() == itr)
			{
				return nullptr;
			}

			itr++;
			if (siblings.end() == itr)
			{
				return nullptr;
			}

			return *itr;
		}

		TreeItemPtr GetParentItem(TreeItemPtr item)
		{
			auto itr = m_items.find(item);
			if (m_items.end() == itr)
			{
				return nullptr;
			}

			return itr->second;
		}

		TreeItemPtr GetChildItem(TreeItemPtr item)
		{
			const auto& children = GetChildren(item);
			if (children.empty())
			{
				return nullptr;
			}

			return children.front();
		}
		
		bool ItemHasChildren(TreeItemPtr item)
		{
			const auto& children = GetChildren(item);

			return !children.empty();
		}

	public:
		void InsertItem(TreeItemPtr item, TreeItemPtr parentItem = nullptr, TreeItemPtr after = nullptr)
		{
			m_items.insert(std::make_pair(item, parentItem));

			auto itr = m_parents.find(parentItem);
			if (m_parents.end() != itr)
			{
				std::list<TreeItemPtr>& children = itr->second;
				if (after == nullptr)
				{
					children.push_back(item);
				}
				else if (parentItem == after)
				{
					children.push_front(item);
				}
				else
				{
					auto itrChild = std::find(children.begin(), children.end(), after);
					if (itrChild == children.end())
					{
						children.push_back(item);
					}
					else
					{
						children.insert(itrChild, item);
					}
				}
			}
			else
			{
				std::list<TreeItemPtr> children;
				children.push_back(item);
				m_parents.insert(std::make_pair(parentItem, children));
			}
		}

		int DeleteItem(TreeItemPtr item, bool delChildren = true)
		{
			int nCount = 0;
			if (delChildren)
			{
				nCount += DeleteChildren(item);
			}

			auto itr = m_items.find(item);
			if (itr != m_items.end())
			{
				m_items.erase(item);
				nCount++;
			}

			return nCount;
		}

		void DeleteItems()
		{
			m_items.clear();
			m_parents.clear();
		}

		int DeleteChildren(TreeItemPtr item)
		{
			auto itr = m_parents.find(item);
			if (m_parents.end() == itr)
			{
				return 0;
			}

			int nCount = 0;
			std::list<TreeItemPtr>& children = itr->second;
			for (TreeItemPtr child : children)
			{
				nCount += DeleteItem(child, true);
			}

			m_parents.erase(item);

			return nCount;
		}

	protected:
		std::map<TreeItemPtr, TreeItemPtr> m_items;
		std::map<TreeItemPtr, std::list<TreeItemPtr>> m_parents;
	};

}