#pragma once

class Buffer final
{
public:

	Buffer() = default;

	Buffer(const Buffer& other)
	{
		if (this == &other)
			return;

		this->m_Data = other.m_Data;
		this->m_SizeBytes = other.m_SizeBytes;

		// #TODO Investigate proper way to copy buffer references, or smart pointer usage
		other.m_OwnershipTransferred = true;
		this->m_OwnershipTransferred = false;
	}

	Buffer(uint64_t bytes)
	{
		Allocate(bytes);
	}

	virtual ~Buffer()
	{
		if (!m_OwnershipTransferred)
		{
			Release();
		}
	}

	void Allocate(uint64_t bytes)
	{
		m_Data = new uint8_t[bytes];
		m_SizeBytes = bytes;
	}

	void Release()
	{
		if (m_SizeBytes > 0)
		{
			delete[] m_Data;
			m_SizeBytes = 0;
			m_OwnershipTransferred = false;
		}
	}

	void Fill(const char byteToWrite)
	{
		if (m_SizeBytes > 0)
		{
			memset((char*)m_Data, byteToWrite, m_SizeBytes);
		}
	}

	void Write(Buffer other)
	{
		uint64_t bytesToWrite = m_SizeBytes;
		if (other.m_SizeBytes < bytesToWrite)
		{
			bytesToWrite = other.m_SizeBytes;
		}
		memcpy((char*)m_Data, other.Data(), bytesToWrite);
	}

	uint8_t* Data() { return m_Data; }
	uint64_t SizeInBytes() { return m_SizeBytes; }

	template <typename T>
	T* As()
	{
		// #TODO Expirment with: return static_cast<T*>(m_Data);
		return (T*)m_Data;
	}

	Buffer& operator=(const Buffer& other)
	{
		if (this == &other)
			return *this;

		Release();

		this->m_Data = other.m_Data;
		this->m_SizeBytes = other.m_SizeBytes;

		// #TODO Investigate proper way to copy buffer references, or smart pointer usage
		other.m_OwnershipTransferred = true;
		this->m_OwnershipTransferred = false;

		return *this;
	}

	operator bool() const
	{
		return m_Data != nullptr;
	}

protected:
	uint8_t* m_Data = nullptr;
	uint64_t m_SizeBytes = 0;

	mutable bool m_OwnershipTransferred = false;
};

