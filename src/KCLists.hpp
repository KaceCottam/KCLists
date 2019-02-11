#ifndef KC_LISTS_H
#define KC_LISTS_H

#include <iostream>
#include <utility>

namespace KC
{
	template <typename T>
	class ListNode final
	{
	public:
		T Data;
		ListNode<T>* Next;
		ListNode<T>* Previous;

		ListNode();
		ListNode(ListNode<T> const& other);
		ListNode(ListNode<T>&& other) noexcept;
		~ListNode();
		explicit ListNode(T const& data);

		auto operator=(T const& data) -> ListNode<T>&;
		auto operator=(ListNode<T> const& other) -> ListNode<T>&;
		auto operator=(ListNode<T>&& other) noexcept -> ListNode<T>&;
	};

	template <class T>
	class List
	{
	protected:
		ListNode<T>* Header;
		int Length;
	public:
		List();
		List(int length, T const* data);
		explicit List(T const& data);
		explicit List(std::initializer_list<T> data);

		List(List<T> const& other);
		List(List<T>&& other) noexcept;

		auto GetHeader() const -> ListNode<T>&;
		auto GetIndex(int index) const -> T&;
		auto GetLength() const -> int;

		auto Push(T const& data) -> void; // operator<<()
		auto Push(int length, T const* data) -> void;
		auto Push(std::initializer_list<T> data) -> void;

		auto Pull() -> T; // operator>>()

		auto operator=(List<T> const& other) -> List<T>&;
		auto operator=(List<T>&& other) noexcept -> List<T>&;

		auto operator<<(T const& data) -> List<T>&;
		auto operator<<(std::initializer_list<T> data) -> List<T>&;
		auto operator>>(T& data) -> List<T>&;

		auto operator[](int index) const -> T&;

		~List();
	};
}

template <typename T>
KC::ListNode<T>::ListNode() : Next(nullptr), Previous(nullptr)
{
}

template <typename T>
KC::ListNode<T>::ListNode(ListNode<T> const& other) : Data(other.Data), Next(nullptr), Previous(nullptr)
{
}

template <typename T>
KC::ListNode<T>::ListNode(ListNode<T>&& other) noexcept
{
	Data = other.Data;
	Next = other.Next;
	Previous = other.Previous;
	other.Data = 0;
	other.Next = nullptr;
	other.Previous = nullptr;
}

template <typename T>
KC::ListNode<T>::ListNode(T const& data) : Data{ data }, Next(nullptr), Previous(nullptr)
{
}

template <typename T>
KC::ListNode<T>::~ListNode() = default;

template <typename T>
auto KC::ListNode<T>::operator=(T const& data) -> ListNode<T>&
{
	this->Data = data;
	return *this;
}

template <typename T>
auto KC::ListNode<T>::operator=(ListNode<T> const& other) -> ListNode<T>&
{
	this->Data = other.Data;
	return *this;
}

template <typename T>
auto KC::ListNode<T>::operator=(ListNode<T>&& other) noexcept -> ListNode<T>&
{
	if (this != &other)
	{
		Data = other.Data;
		other.Data = 0;
	}
	return *this;
}

template <class T>
KC::List<T>::List() : Header{ nullptr }, Length(0)
{
}

template <class T>
KC::List<T>::List(List<T>&& other) noexcept
{
	Header = other.Header;
	Length = other.Length;
	other.Header = nullptr;
	other.Length = 0;
}

template <class T>
auto KC::List<T>::GetHeader() const -> ListNode<T>&
{
	return *Header;
}

template <class T>
KC::List<T>::List(List<T> const& other) : Header(new ListNode<T>(other.Header->Data)), Length(1)
{
	auto length = other.GetLength();
	for (auto i = 1; i < length; i++)
	{
		this->Push(other.GetIndex(i));
	}
}

template <class T>
KC::List<T>::List(T const& data) : Header{ new ListNode<T>(data) }, Length(1)
{
}

template <class T>
KC::List<T>::List(std::initializer_list<T> data) : Header(new ListNode<T>(*(data.end() - 1))), Length(1)
{
	auto length = data.size() - 1;
	this->Push(length, data.begin());
}

template <class T>
KC::List<T>::List(const int length, T const* data) : Header{ new ListNode<T>{ data[0] } }, Length(1)
{
	for (auto i = 1; i < length; i++)
	{
		Push(data[i]);
	}
}

template <class T>
auto KC::List<T>::GetLength() const -> int
{
	return Length;
}

template <class T>
auto KC::List<T>::Push(T const& data) -> void
{
	ListNode<T>* previousHeader = Header;
	Header = new ListNode<T>(data);
	if (previousHeader)
	{
		Header->Next = previousHeader;
		previousHeader->Previous = Header;
	}
	Length++;
}

template <class T>
auto KC::List<T>::Push(const int length, T const* data) -> void
{
	for (auto i = length - 1; i >= 0; --i)
	{
		Push(data[i]);
	}
}

template <class T>
auto KC::List<T>::Push(std::initializer_list<T> data) -> void
{
	auto length = data.size();
	Push(length, data.begin());
}

template <class T>
auto KC::List<T>::Pull() -> T
{
	if (!Header)
		return 0;
	T data = Header->Data;
	ListNode<T>* oldHeader = Header;
	Header = Header->Next;
	delete oldHeader;
	Length--;
	return data;
}


template <class T>
auto KC::List<T>::GetIndex(const int index) const -> T&
{
	ListNode<T>* traversalNode = Header;
	for (auto i = 0; i < index && traversalNode->Next; i++)
	{
		traversalNode = traversalNode->Next;
	}
	return traversalNode->Data;
}

template <class T>
KC::List<T>::~List()
{
	while (Header)
	{
		Pull();
	}
}

template <class T>
auto KC::List<T>::operator=(List<T> const& other) -> List<T>&
{
	while (Header)
	{
		Pull();
	}
	Header = new ListNode<T>(other.Header->Data);
	auto length = other.Length;
	for (auto i = length; i > 1; --i)
	{
		this->Push(other.GetIndex(i));
	}
	return *this;
}

template <class T>
auto KC::List<T>::operator=(List<T>&& other) noexcept -> List<T>&
{
	if (this != &other)
	{
		Header = other.Header;
		Length = other.Length;
		other.Header = nullptr;
		other.Length = 0;
	}
	return *this;
}

template <class T>
auto KC::List<T>::operator<<(T const& data) -> List<T>&
{
	Push(data);
	return *this;
}

template <class T>
auto KC::List<T>::operator >> (T& data) -> List<T>&
{
	data = Pull();
	return *this;
}

template <class T>
auto KC::List<T>::operator<<(std::initializer_list<T> data) -> List<T>&
{
	Push(data);
	return *this;
}

template <class T>
auto KC::List<T>::operator[](const int index) const -> T&
{
	if (Length - 1 > index)
	{
		throw std::out_of_range("Index is greater than the length of list!");
	}
	return GetIndex(index);
}

template <typename T>
auto operator<<(std::ostream& stream, const KC::ListNode<T>& node) -> std::ostream&
{
	std::cout << node.Data;
	return stream;
}

template <typename T>
auto operator<<(std::ostream& stream, const KC::List<T>& list) -> std::ostream&
{
	auto length = list.GetLength();
	for (auto i = 0; i < length; i++)
	{
		std::cout << "[" << i << ":$" << &list.GetIndex(i) << "] " << list.GetIndex(i) << std::endl;
	}
	return stream;
}

#endif // !KC_LISTS_H
