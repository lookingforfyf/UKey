/*! \file lgntlv.h
*  \brief TLV、LV报文类
*
*  工程要包含本文件，依赖文件：lgnbase.h
*
*  提供TLV、LV报文的组包和解包；用户可以自定义数据结构
*  
*  \author ljc
*  \version 3.0.0
*  \date    2010-2012
*  \bug
*  \warning
*/

#ifndef __LGN_TLV_H__
#define __LGN_TLV_H__

#ifndef __LGN_BASE_H__
#error lgnthread.h requires include lgnbase.h
#endif


namespace LGN
{
	//! LittleEndian 低地址存放最低有效字节
	class Convert_LE
	{
	public:
		static void Uint16ToBytes(UINT16 unNum, LPBYTE pbBuff)
		{
			pbBuff[0] = (BYTE)(unNum);
			pbBuff[1] = (BYTE)(unNum >> 8);
		}

		static UINT16 Uint16FromBytes(LPCBYTE pbBuff)
		{
			return (pbBuff[0]) + (pbBuff[1] << 8);
		}


		static void Uint32ToBytes(UINT32 unNum, LPBYTE pbBuff)
		{
#if (LGN_SUPPORT_LITTLEENDIAN)
			API::memcpy(pbBuff, &unNum, 4);
#else
			pbBuff[0] = (BYTE)(unNum);
			pbBuff[1] = (BYTE)(unNum >> 8);
			pbBuff[2] = (BYTE)(unNum >> 16);
			pbBuff[3] = (BYTE)(unNum >> 24);
#endif
		}

		static UINT32 Uint32FromBytes(LPCBYTE pbBuff)
		{
#if (LGN_SUPPORT_LITTLEENDIAN)
			return *((LPUINT32)pbBuff);
#else
			return (pbBuff[0]) + (pbBuff[1] << 8) + (pbBuff[2] << 16) + (pbBuff[3] << 24);
#endif
		}

	};

	//! BigEndian 低地址存放最高有效字节
	class Convert_BE
	{
	public:
		static void Uint16ToBytes(UINT16 unNum, LPBYTE pbBuff)
		{
			pbBuff[0] = (BYTE)(unNum >> 8);
			pbBuff[1] = (BYTE)(unNum);
		}

		static UINT16 Uint16FromBytes(LPCBYTE pbBuff)
		{
			return (pbBuff[0] << 8) + (pbBuff[1]);
		}


		static void Uint32ToBytes(UINT32 unNum, LPBYTE pbBuff)
		{
			pbBuff[0] = (BYTE)(unNum >> 24);
			pbBuff[1] = (BYTE)(unNum >> 16);
			pbBuff[2] = (BYTE)(unNum >> 8);
			pbBuff[3] = (BYTE)(unNum);
		}

		static UINT32 Uint32FromBytes(LPCBYTE pbBuff)
		{
			return (pbBuff[0] << 24) + (pbBuff[1] << 16) + (pbBuff[2] << 8) + (pbBuff[3]);
		}

	};



	//! LV（长度+数据）数据结构
	struct LVITEM
	{
		//! 长度
		UINT32 unLength;
		//! 数据指针
		LPVOID pData;

		//! 常量
		enum {
			LENSIZE = 2 //!< 长度项占用字节数
		};

		/*! \brief 长度转换，把整数值转成字节流（跟平台无关）
			\param [in] unLength 长度的整数值
			\param [out] pbLenBuff 长度的字节流
			\return 长度项占用字节数（LENSIZE）
		*/
		static UINT32 ConvertLength(UINT32 unLength, LPBYTE pbLenBuff)
		{
			Convert_BE::Uint16ToBytes((UINT16)unLength, pbLenBuff);
			return LVITEM::LENSIZE;
		}

		/*! \brief 长度转换，把字节流转成整数值（跟平台无关）
			\param [in] pbLenBuff 长度的字节流
			\return 长度的整数值
		*/
		static UINT32 ConvertLength(LPCBYTE pbLenBuff)
		{
			return Convert_BE::Uint16FromBytes(pbLenBuff);
		}
	};

	//! LV创建者模板类
	template< typename T >
	class CLVCreatorT
	{
	public:
		CLVCreatorT()
		{
		}

		CLVCreatorT(const CLVCreatorT & o)
		{
			m_lvBuff = o.m_lvBuff;
		}

		CLVCreatorT(const T * pItems, int nCount=1)
		{
			AppendItems(pItems, nCount);
		}

		CLVCreatorT& operator=( const CLVCreatorT& o )
		{
			m_lvBuff = o.m_lvBuff;
			return (*this);
		}

		//! 增加一个数据项
		void Append(UINT32 unItemLen, LPVOID pItemData)
		{
			BYTE abLenBuff[16];
			T::ConvertLength(unItemLen, abLenBuff);

			m_lvBuff.Append(abLenBuff, T::LENSIZE);
			m_lvBuff.Append((LPBYTE)pItemData, unItemLen);
		}

		//! 增加多个数据项
		void AppendItems(const T * pItems, int nCount=1)
		{
			LGNASSERT( pItems != NULL );

			for (int i=0; i<nCount; i++)
			{
				Append(pItems[i].unLength, pItems[i].pData);
			}
		}

		LGN_INLINE const CByteBuffer GetPacket() const
		{
			return m_lvBuff;
		}
		LGN_INLINE void GetPacket(LPBYTE pbPacket, LPUINT32 punPacketLen) const
		{
			if (pbPacket != NULL)
			{
				API::memcpy(pbPacket, (LPCBYTE)m_lvBuff, m_lvBuff.GetLength());
			}
			*punPacketLen = m_lvBuff.GetLength();
		}

		LGN_INLINE UINT32 GetPacketLength()
		{
			return m_lvBuff.GetLength();
		}

		LGN_INLINE LPCBYTE GetPacketBuffer() const
		{
			return (LPCBYTE)m_lvBuff;
		}


	protected:
		CByteBuffer m_lvBuff;
	};

	typedef CLVCreatorT< LVITEM > CLVCreator;


	//! LV查找者模板类
	template< typename T >
	class CLVFinderT
	{
	public:
		CLVFinderT()
		{
		}

		CLVFinderT(const CByteBuffer & packet) throw()
		{
			if (!TryParse((LPCBYTE)packet, (UINT32)packet.GetLength()))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
		}

		CLVFinderT(LPCBYTE pbPacket, UINT32 unPacketLen) throw()
		{
			if (!TryParse(pbPacket, unPacketLen))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
		}

		//! 解析LV数据包，返回是否成功
		bool TryParse(LPCBYTE pbPacket, UINT32 unPacketLen)
		{
			T item;
			for (LPCBYTE p = pbPacket; p < (pbPacket + unPacketLen); )
			{
				item.unLength = T::ConvertLength(p);
				p += T::LENSIZE;
				if ((p + item.unLength) > (pbPacket + unPacketLen))
				{
					return false;
				}
				item.pData = (LPVOID)p;
				p += item.unLength;

				m_lvHeader.Append(&item, 1);
			}
			return true;
		}

		CBufferT< T > Parse(const CByteBuffer & packet) throw()
		{
			if (!TryParse((LPCBYTE)packet, (UINT32)packet.GetLength()))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
			return m_lvHeader;
		}

		//! 解析LV数据包，返回LV数据项数组
		CBufferT< T > Parse(LPCBYTE pbPacket, UINT32 unPacketLen) throw()
		{
			if (!TryParse(pbPacket, unPacketLen))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
			return m_lvHeader;
		}

		LGN_INLINE const T * GetItems() const
		{
			return (const T *)m_lvHeader;
		}

		LGN_INLINE int GetCount() const
		{
			return m_lvHeader.GetLength();
		}

	protected:
		CBufferT< T > m_lvHeader;

	private:
		//! 不允许复制或重构造
		CLVFinderT(const CLVFinderT & o);
		CLVFinderT& operator=( const CLVFinderT& o );
	};

	typedef CLVFinderT< LVITEM > CLVFinder;



////////////////////////////////////////////////////////////////////////////////////////////////////
	//! TLV（标签+长度+数据）数据结构。点击这里查看 \ref tlv_example_page
	struct TLVITEM
	{
		//! 标签
		UINT32 unTag;
		//! 长度
		UINT32 unLength;
		//! 数据指针
		LPVOID pData;

		//! 常量
		enum {
			TAGSIZE = 4, //!< 标签项占用字节数
			LENSIZE = 4 //!< 长度项占用字节数
		};

		/*! \brief 长度转换，把整数值转成字节流（跟平台无关）
			\param [in] unLength 长度的整数值
			\param [out] pbLenBuff 长度的字节流
			\return 长度项占用字节数（LENSIZE）
		*/
		static UINT32 ConvertLength(UINT32 unLength, LPBYTE pbLenBuff)
		{
			Convert_LE::Uint32ToBytes(unLength, pbLenBuff);
			return LENSIZE;
		}
		static UINT32 ConvertTag(UINT32 unTag, LPBYTE pbTagBuff)
		{
			Convert_LE::Uint32ToBytes(unTag, pbTagBuff);
			return LENSIZE;
		}

		/*! \brief 长度转换，把字节流转成整数值（跟平台无关）
			\param [in] pbLenBuff 长度的字节流
			\return 长度的整数值
		*/
		static UINT32 ConvertLength(LPCBYTE pbLenBuff)
		{
			return Convert_LE::Uint32FromBytes(pbLenBuff);
		}
		static UINT32 ConvertTag(LPCBYTE pbTagBuff)
		{
			return Convert_LE::Uint32FromBytes(pbTagBuff);
		}
	};

	//! TLV创建者模板类。点击这里查看 \ref tlv_example_page
	template< typename T >
	class CTLVCreatorT
	{
	public:
		CTLVCreatorT()
		{
		}

		CTLVCreatorT(const CTLVCreatorT & o)
		{
			m_tlvBuff = o.m_tlvBuff;
		}

		CTLVCreatorT(const T * pItems, int nCount=1)
		{
			AppendItems(pItems, nCount);
		}

		CTLVCreatorT& operator=( const CTLVCreatorT& o )
		{
			m_tlvBuff = o.m_tlvBuff;
			return (*this);
		}

		//! 增加一个数据项
		virtual void Append(UINT32 unItamTag, UINT32 unItemLen, LPVOID pItemData)
		{
			BYTE abTagBuff[16], abLenBuff[16];
			T::ConvertTag(unItamTag, abTagBuff);
			T::ConvertLength(unItemLen, abLenBuff);

			m_tlvBuff.Append(abTagBuff, T::TAGSIZE);
			m_tlvBuff.Append(abLenBuff, T::LENSIZE);
			m_tlvBuff.Append((LPBYTE)pItemData, unItemLen);
		}

		//! 增加多个数据项
		void AppendItems(const T * pItems, int nCount=1)
		{
			LGNASSERT( pItems != NULL );

			for (int i=0; i<nCount; i++)
			{
				Append(pItems[i].unTag, pItems[i].unLength, pItems[i].pData);
			}
		}

		LGN_INLINE const CByteBuffer GetPacket() const
		{
			return m_tlvBuff;
		}

		LGN_INLINE UINT32 GetPacketLength()
		{
			return m_tlvBuff.GetLength();
		}

		LGN_INLINE LPCBYTE GetPacketBuffer() const
		{
			return (LPCBYTE)m_tlvBuff;
		}


	protected:
		CByteBuffer m_tlvBuff;
	};

	typedef CTLVCreatorT< TLVITEM > CTLVCreator;


	//! TLV查找者模板类。点击这里查看 \ref tlv_example_page
	template< typename T >
	class CTLVFinderT
	{
	public:
		CTLVFinderT()
		{
		}

		CTLVFinderT(const CByteBuffer & packet) throw()
		{
			if (!TryParse((LPCBYTE)packet, (UINT32)packet.GetLength()))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
		}

		CTLVFinderT(LPCBYTE pbPacket, UINT32 unPacketLen) throw()
		{
			if (!TryParse(pbPacket, unPacketLen))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
		}

		//! 解析TLV数据包，返回是否成功
		virtual bool TryParse(LPCBYTE pbPacket, UINT32 unPacketLen)
		{
			T item;
			for (LPCBYTE p = pbPacket; p < (pbPacket + unPacketLen); )
			{
				item.unTag = T::ConvertTag(p);
				p += T::TAGSIZE;
				item.unLength = T::ConvertLength(p);
				p += T::LENSIZE;
				if ((p + item.unLength) > (pbPacket + unPacketLen))
				{
					return false;
				}
				item.pData = (LPVOID)p;
				p += item.unLength;

				m_tlvHeader.Append(&item, 1);
			}
			return true;
		}

		const CBufferT< T > & Parse(const CByteBuffer & packet) throw()
		{
			if (!TryParse((LPCBYTE)packet, (UINT32)packet.GetLength()))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
			return m_tlvHeader;
		}

		//! 解析LV数据包，返回LV数据项数组
		const CBufferT< T > & Parse(LPCBYTE pbPacket, UINT32 unPacketLen) throw()
		{
			if (!TryParse(pbPacket, unPacketLen))
			{
				LGN::LgnThrow(E_LGN_INVALID_DATA);
			}
			return m_tlvHeader;
		}

		LGN_INLINE const T * GetItems() const
		{
			return (const T *)m_tlvHeader;
		}

		LGN_INLINE int GetCount() const
		{
			return m_tlvHeader.GetLength();
		}

		LGN_INLINE const T * Find(UINT32 unFindTag)
		{
			return Find(GetItems(), GetCount(), unFindTag);
		}
		LGN_INLINE bool Find(T * pFindItems, int nFindCount)
		{
			return Find(GetItems(), GetCount(), pFindItems, nFindCount);
		}

	public:
		static const T * Find(const T * pItems, int nCount, UINT32 unFindTag)
		{
			for (int i=0; i<nCount; i++)
			{
				if (pItems[i].unTag == unFindTag)
				{
					return pItems+i;
				}
			}
			return NULL;
		}
		static bool Find(const T * pItems, int nCount, T * pFindItems, int nFindCount)
		{
			bool bResult = true;
			for (int i=0; i<nFindCount; i++)
			{
				const T * pItem = Find(pItems, nCount, pFindItems[i].unTag);
				if (pItem == NULL)
				{
					pFindItems[i].unLength = -1;
					bResult = false;
				}
				else
				{
					if (pFindItems[i].pData != NULL)
					{
						if (pFindItems[i].unLength < pItem->unLength)
						{
							pFindItems[i].unLength = 0;
							bResult = false;
						}
						else
						{
							API::memcpy(pFindItems[i].pData, pItem->pData, pItem->unLength);
							pFindItems[i].unLength = pItem->unLength;
						}
					}
					else
					{
						pFindItems[i].unLength = pItem->unLength;
					}
				}
			}
			return bResult;
		}

	protected:
		CBufferT< T > m_tlvHeader;

	private:
		//! 不允许复制或重构造
		CTLVFinderT(const CTLVFinderT & o);
		CTLVFinderT& operator=( const CTLVFinderT& o );
	};

	typedef CTLVFinderT< TLVITEM > CTLVFinder;


/*! \page tlv_example_page P11属性存储例子
<pre>
\#include "lgntlv.h"

namespace OnKey
{
\#if (LGN_SUPPORT_LITTLEENDIAN)
	typedef LGN::CTLVCreator CP11TLVCreator;
	typedef LGN::CTLVFinder CP11TLVFinder;
\#else

	class CP11TLVCreator : public LGN::CTLVCreator
	{
	public:
		void Append(UINT32 unItamTag, UINT32 unItemLen, LPVOID pItemData)
		{
			if (unItamTag == 1) // 整数型的P11属性，需要调整字节流
			{
				BYTE ab[4];
				LGN::Convert_LE::Uint32ToBytes(*((LPUINT32)pItemData), ab);
				LGN::CTLVCreator::Append(unItamTag, unItemLen, ab);
			}
			else
			{
				LGN::CTLVCreator::Append(unItamTag, unItemLen, pItemData);
			}
		}
	};

	class CP11TLVFinder : public LGN::CTLVFinder
	{
	public:
		bool TryParse(LPCBYTE pbPacket, UINT32 unPacketLen)
		{
			LPBYTE pbBuff = m_tlvBuff.GetBufferSetLength(unPacketLen);
			LGN::API::memcpy(pbBuff, pbPacket, unPacketLen);

			if (!LGN::CTLVFinder::TryParse(pbBuff, unPacketLen))
			{
				return false;
			}

			const LGN::TLVITEM * pItems = m_tlvHeader;
			for (int i=0; i<m_tlvHeader.GetLength(); i++)
			{
				if (pItems[i].unTag == 1)  // 整数型的P11属性，需要调整字节流
				{
					UINT32 un = LGN::Convert_LE::Uint32FromBytes((LPCBYTE)(pItems[i].pData));
					LGN::API::memcpy(pItems[i].pData, &un, 4);
				}
			}
			return true;
		}
	protected:
		LGN::CByteBuffer m_tlvBuff;
	};
\#endif


	class CP11TLVList
	{
	public:
		CP11TLVList()
		{
			m_unBuffOffset = 0;
		}

		CP11TLVList(const CP11TLVList & o)
		{
			m_tlvItems = o.m_tlvItems;
			m_tlvBuff = o.m_tlvBuff;
			m_unBuffOffset = o.m_unBuffOffset;
		}

		CP11TLVList& operator=( const CP11TLVList& o )
		{
			m_tlvItems = o.m_tlvItems;
			m_tlvBuff = o.m_tlvBuff;
			m_unBuffOffset = o.m_unBuffOffset;
			return (*this);
		}

		//! 增加一个数据项（如果存在则覆盖）
		void Append(UINT32 unItamTag, UINT32 unItemLen, LPVOID pItemData)
		{		
			// 该TAG是否存在
			int nIndex = Find_ReturnIndex(unItamTag);
			if (nIndex < 0)
			{
				Append_Tail(unItamTag, unItemLen, pItemData);
			}
			else
			{
				Modify_OfIndex(nIndex, unItamTag, unItemLen, pItemData);
			}
		}

		//! 增加多个数据项（如果存在则覆盖）
		void AppendItems(const LGN::TLVITEM * pItems, int nCount=1)
		{
			LGNASSERT( pItems != NULL );

			for (int i=0; i<nCount; i++)
			{
				Append(pItems[i].unTag, pItems[i].unLength, pItems[i].pData);
			}
		}

		//! 包含多个数据项
		bool ContainItems(const LGN::TLVITEM * pSubItems, int nCount=1)
		{
			LGNASSERT( pSubItems != NULL );

			const LGN::TLVITEM * pItems = (const LGN::TLVITEM *)m_tlvItems;
			for (int i=0; i<nCount; i++)
			{
				int nIndex = Find_ReturnIndex(pSubItems[i].unTag);
				if (nIndex < 0)
				{
					return false;
				}				
				if (pItems[nIndex].unLength != pSubItems[i].unLength)
				{
					return false;
				}
				if (LGN::API::memcmp(pItems[nIndex].pData, pSubItems[i].pData, pSubItems[i].unLength) != 0)
				{
					return false;
				}
			}
			return true;
		}

		LGN::CByteBuffer Encode()
		{
			CP11TLVCreator tlvCreator;
			tlvCreator.AppendItems(m_tlvItems, m_tlvItems.GetLength());
			return tlvCreator.GetPacket();
		}

		void Decode(const LGN::CByteBuffer & packet)  throw()
		{
			Decode(packet, (UINT32)packet.GetLength());
		}
		void Decode(LPCBYTE pbPacket, UINT32 unPacketLen)  throw()
		{
			CP11TLVFinder tlvFinder;
			tlvFinder.Parse(pbPacket, unPacketLen);

			m_tlvItems.Empty();
			m_tlvBuff.Empty();
			m_unBuffOffset = 0;
			AppendItems(tlvFinder.GetItems(), tlvFinder.GetCount());
		}

	protected:
		int Find_ReturnIndex(UINT32 unItemTag)
		{
			int nCount = m_tlvItems.GetLength();
			const LGN::TLVITEM * pItems = (const LGN::TLVITEM *)m_tlvItems;
			for (int i=0; i<nCount; i++)
			{
				if (pItems[i].unTag == unItemTag)
				{
					return i;
				}
			}
			return -1;
		}

		void Append_Tail(UINT32 unItamTag, UINT32 unItemLen, LPVOID pItemData)
		{			
			LGN::TLVITEM item;
			item.unTag = unItamTag;
			item.unLength = unItemLen;
			item.pData = GetTLVBuffer(m_unBuffOffset + unItemLen) + m_unBuffOffset; // 重新分配空间，指针重指向

			LGN::API::memcpy(item.pData, pItemData, unItemLen);
			m_unBuffOffset += unItemLen;
			
			m_tlvItems.Append(&item, 1);
		}

		void Modify_OfIndex(int nIndex, INT32 unItamTag, UINT32 unItemLen, LPVOID pItemData)
		{
			LGN::TLVITEM * pItem = m_tlvItems.GetBuffer() + nIndex;
			if (unItemLen > pItem->unLength)
			{
				pItem->unLength = unItemLen;
				// 重新分配空间，指针重指向
				pItem->pData = GetTLVBuffer(m_unBuffOffset + unItemLen) + m_unBuffOffset;

				LGN::API::memcpy(pItem->pData, pItemData, unItemLen);
				m_unBuffOffset += unItemLen;
			}
			else
			{
				pItem->unLength = unItemLen;
				LGN::API::memcpy(pItem->pData, pItemData, unItemLen);
			}
		}

		LPBYTE GetTLVBuffer(UINT32 unNewAllocSize)
		{
			int nNewSize = (unNewAllocSize + 0xFF) & 0xFFFFFF00;
			if (m_tlvBuff.GetAllocLength() < nNewSize)
			{
				LPCBYTE pbOrgBuff = m_tlvBuff;
				LPBYTE pbNewBuff = m_tlvBuff.GetBuffer(nNewSize);

				// 指针重指向的新分配内存
				int nCount = m_tlvItems.GetLength();
				LGN::TLVITEM * pItems = m_tlvItems.GetBuffer();
				for (int i=0; i<nCount; i++)
				{
					LPCBYTE p = (LPCBYTE)(pItems[i].pData);
					pItems[i].pData = (LPVOID)(pbNewBuff + (p - pbOrgBuff));
				}
				//m_tlvBuff.ReleaseBufferSetLength(nOrgLength);
				return pbNewBuff;
			}
			else
			{
				return m_tlvBuff.GetBuffer();
			}
		}

	protected:
		LGN::CBufferT< LGN::TLVITEM > m_tlvItems;
		LGN::CByteBuffer m_tlvBuff;
		UINT32 m_unBuffOffset;
	};

};

//...
	UINT32 un = 0x11223344;
	LGN::TLVITEM items[2] = {{1, 4, &un}, {0x11223344, 5, "12345"}};
	OnKey::CP11TLVCreator tlvCreator;
	tlvCreator.AppendItems(items, 2);

	OnKey::CP11TLVFinder tlvFinder;
	const LGN::TLVITEM * pItems = tlvFinder.Parse(tlvCreator.GetPacket());

</pre>
*/

};

#endif // __LGN_TLV_H__

