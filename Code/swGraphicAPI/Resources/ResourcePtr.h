#pragma once

#include <type_traits>


class ResourceObject;


/**@brief Wrapper for low level resources and high level assets.

This class automatically increments and decrements resources and assets reference counter.
Notice that it's not equivalent of std::shared_ptr. When references counter reaches 0, resource won't be released.
@ref AssetsManager is the only one owner of resource and it's responsibility is, to destroy resource
when it's needed. Even when there's no Actor in engine that uses it, resource can still remain for future use.

@ingroup Resources*/
template< typename ResourceType >
class ResourcePtr
{
private:
	ResourceType*		m_resource;

public:
	ResourcePtr()
	{
		static_assert( std::is_base_of< ResourceObject, ResourceType >::value, "Template parameter type must inherit from ResourceObject" );
		m_resource = nullptr;
	}

	ResourcePtr( ResourceType* ptr )
	{
		static_assert( std::is_base_of< ResourceObject, ResourceType >::value, "Template parameter type must inherit from ResourceObject" );
		m_resource = ptr;
	}

	~ResourcePtr()
	{
		ReleaseResource();
	}

	ResourcePtr( const ResourcePtr& other )
	{
		m_resource = nullptr;
		AssignPointer( other.m_resource );
	}

	ResourcePtr( ResourcePtr&& other )
	{
		if( this != &other)
		{
			m_resource = other.m_resource;
			other.m_resource = nullptr;
		}
	}

	void operator=( ResourceType* ptr )
	{
		ReleaseResource();
		AssignPointer( ptr );
	}

	void operator=( const ResourcePtr< ResourceType >& ptr )
	{
		ReleaseResource();
		AssignPointer( ptr.m_resource );
	}

	operator void*() const
	{
		return m_resource;
	}

	ResourceType* operator*()
	{
		return m_resource;
	}

	ResourceType* operator->()
	{
		return m_resource;
	}

	const ResourceType* operator*() const
	{
		return m_resource;
	}

	const ResourceType* operator->() const
	{
		return m_resource;
	}

	void ReleaseResource()
	{
		if( m_resource )
			m_resource->DeleteObjectReference();
		m_resource = nullptr;
	}

	void AssignPointer( ResourceType* ptr )
	{
		if( ptr )
		{
			m_resource = ptr;
			m_resource->AddObjectReference();
		}
	}

	ResourceType*	Ptr	() const
	{ return m_resource; }

};



namespace rttr
{

// This part of code enables ResourcePtr as wrapper for rttr system.


template< typename T >
struct wrapper_mapper< ResourcePtr< T > >
{
    using wrapped_type  = decltype( std::declval< ResourcePtr< T > >().Ptr() );
    using type          = ResourcePtr< T >;

    inline static wrapped_type	get		( const type& obj )
    {
       return obj.Ptr();
    }

    inline static type			create	( const wrapped_type& value )
    {
       return ResourcePtr< T >( value );
    } 
};


} // end namespace rttr
