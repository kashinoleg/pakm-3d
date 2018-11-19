#pragma once

namespace TexGen { 
	/// Template used as a base class for creating singletons
	/**
	Declare a singleton class as
	class CMyClass : public CSingleton<CMyClass>
	To declare the single instance this line should be put at the top of the .cpp file
	template<> CMyClass CSingleton<CMyClass>::m_SingleInstance = CMyClass();
	*/
	template< typename T >
	class CLASS_DECLSPEC CSingleton
	{
	public:
		CSingleton()
		{
			// There should only be one instance created, only use static GetInstance() function
			// instead of instantiating manually
			assert(this == &CSingleton::GetInstance());
		}
		static T &GetInstance()
		{
			return m_SingleInstance;
		}
		static T m_SingleInstance;
	};
}
