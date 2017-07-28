#ifndef _PKCS11T_TXT_H_
#define _PKCS11T_TXT_H_


// The flags are defined in the following table:
//      Bit Flag                        Mask        Meaning
#define CKF_CSP_P11_INIT				0x80001000	//CSP 调用p11，内部维护
#define CKF_CSP_SESSION					0x00010000  // 总是用一个会话
#define CKF_VERIFYCONTEXT_SESSION		0x80000000	// 不读取令牌信息进行校验，减少指令执行

/////////////////////////////////////////////////////
// tendyron
#define CKA_MY_KEYID           0x80000002
#define CKA_MY_TYPE            0x80000003
#define CKA_MY_NAME            0x80000004
/////////////////////////////////////////////////////

//ckk
#define CKK_TDR_DEFINED			(CKK_VENDOR_DEFINED|0x00001000)
#define CKK_SSF33				(CKK_TDR_DEFINED | 0x00000100)
#define CKK_SCB2				(CKK_TDR_DEFINED | 0x00000200)
#define CKK_SM1					(CKK_TDR_DEFINED | 0x00000400)

//ckm
#define CKM_TDR_DEFINED					(CKM_VENDOR_DEFINED|0x00100000)
#define CKM_TDR_ALG_MASK				0x0FFF0FFF
#define TDR_SSF33_ALG					0x00010000
#define TDR_SCB2_ALG					0x00020000
#define TDR_SM1_ALG						0x00040000

#define TDR_SHAMD5_ALG				    0x00030000
#define TDR_ALG_CBC						0x00000001
#define TDR_ALG_ECB						0x00000002
#define TDR_ALG_MAC						0x00000004
#define	TDR_ALG_PAD						0x00000100

#define CKM_SHAMD5_RSA_PKCS				(CKM_TDR_DEFINED | TDR_SHAMD5_ALG)
#define CKM_SSF33_KEY_GEN				(CKM_TDR_DEFINED | TDR_SSF33_ALG)
#define CKM_SSF33_CBC					(CKM_TDR_DEFINED | TDR_SSF33_ALG | TDR_ALG_CBC)
#define CKM_SSF33_ECB					(CKM_TDR_DEFINED | TDR_SSF33_ALG | TDR_ALG_ECB)
#define CKM_SSF33_MAC					(CKM_TDR_DEFINED | TDR_SSF33_ALG | TDR_ALG_MAC)
#define CKM_SSF33_CBC_PAD				(CKM_TDR_DEFINED | TDR_SSF33_ALG | TDR_ALG_CBC | TDR_ALG_PAD)
#define CKM_SSF33_ECB_PAD				(CKM_TDR_DEFINED | TDR_SSF33_ALG | TDR_ALG_ECB | TDR_ALG_PAD)

#define CKM_SCB2_KEY_GEN				(CKM_TDR_DEFINED | TDR_SCB2_ALG)
#define CKM_SCB2_CBC					(CKM_TDR_DEFINED | TDR_SCB2_ALG | TDR_ALG_CBC)
#define CKM_SCB2_ECB					(CKM_TDR_DEFINED | TDR_SCB2_ALG | TDR_ALG_ECB)
#define CKM_SCB2_MAC					(CKM_TDR_DEFINED | TDR_SCB2_ALG | TDR_ALG_MAC)
#define CKM_SCB2_CBC_PAD				(CKM_TDR_DEFINED | TDR_SCB2_ALG | TDR_ALG_CBC | TDR_ALG_PAD)
#define CKM_SCB2_ECB_PAD				(CKM_TDR_DEFINED | TDR_SCB2_ALG | TDR_ALG_ECB | TDR_ALG_PAD)

// CSP
#define PP_PINPAD_PIN			0x10001 // PINPAD校验密码
#define PP_GET_ALLCERT			0x10002 // 获取所有证书数据

typedef struct _CSP_MULTI_CERTS
{
	DWORD dwSlotId;
	LPBYTE pbNamesBuff;
	DWORD dwNamesLen;
	LPBYTE pbCertsBuff;
	DWORD dwCertsLen;
}CSP_MULTI_CERTS, *PCSP_MULTI_CERTS;

#endif//_PKCS11T_TXT_H_