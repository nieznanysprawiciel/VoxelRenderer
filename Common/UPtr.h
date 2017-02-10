#pragma once
/**
@file UPtr
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/


template< typename WrappedType >
class UPtr
{
private:
	WrappedType*		m_ptr;

public:
	UPtr()
		:	m_ptr( nullptr )
	{}

	UPtr( WrappedType* ptr )
		:	m_ptr( ptr )
	{}

	~UPtr()
	{	ReleaseResource();	}

	UPtr( const UPtr< WrappedType >& other ) = delete;
	UPtr( UPtr< WrappedType >&& other )
	{
		if( this != &other)
		{
			m_resource = other.m_resource;
			other.m_resource = nullptr;
		}
	}


	void operator=( const UPtr< WrappedType >& other ) = delete;

	void operator=( UPtr< WrappedType >&& other )
	{
		ReleaseResource();

		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
	}




	void ReleaseResource()
	{
		if( m_ptr )
			delete m_ptr;
		m_ptr = nullptr;
	}

	void AssignPointer( WrappedType* ptr )
	{
		m_ptr = ptr;
	}
};