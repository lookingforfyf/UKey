#ifndef __ONKEYPREDEF_H__
#define __ONKEYPREDEF_H__


//----------------------------------------------------------------------------------------------------------
// 定义日志级别以及开关
#ifdef _DEBUG
#define ONKEY_LOG_ON					1  				// 日志总开关					
#else
#define ONKEY_LOG_ON					0  				// 日志总开关					
#endif
#define ONKEY_LOG_PATH 					"/tmp/OnKey.log"
#define ONKEY_P11_LOG_ON				1
#define ONKEY_TOKEN_LOG_ON				1	
#define ONKEY_DEV_LOG_ON				1

// --------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
//定义一些常量，
#ifndef WIN32
#define NULL	0
#define OUT 
#define IN 
#define TRUE	1
#define FALSE	0
#ifndef _OBJC_OBJC_H_
typedef bool	BOOL;
#endif

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned char BYTE;
typedef unsigned char *LPBYTE;
typedef DWORD* LPDWORD;
typedef unsigned short* LPWORD;
typedef long LONG;

#endif

// 初始化密钥（16位）。如果没有初始化密钥，屏蔽下一行
#define ONKEY_APP_OEM_INITKEY			"\xB4\x02\xF1\x15\xF5\xFF\xE5\x36\x9F\x37\xAB\xA1\xD2\x4D\x42\x8F"
#define ONKEY_USE_PIN_MIN_LEN			6 // 默认密码最小长度
#define ONKEY_USE_PIN_MAX_LEN			8 // 默认密码最大长度

#define ONKEY_DEFAULT_USER_PIN			"12345678" // 默认密码
#define ONKEY_CONTROL_DEFAULT_PIN		1  // 是否控制默认的密码的开关

#define OPT_SIGN_DIPLAY					1	
#define OPT_SIGN_NORMAL					2
#define OPT_SIGN_MD5					4
#define OPT_READ_BINARY					8
#define OPT_GENERATE_KEYPAIR			16
#define OPT_KEYPAIR_DECRYPT				32

#define SIGN_ACCES						1
#define Read_ACCES						2
#define FF_EXE
// 根证书
# define CCB_ROOT_CERT_DATA		(LPBYTE)("\x30\x82\x02\xb7\x30\x82\x02\x20\xa0\x03\x02\x01\x00\x02\x01\x16\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x04\x05\x00\x30\x81\x8c\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x43\x4e\x31\x0c\x30\x0a\x06\x03\x55\x04\x0a\x13\x03\x43\x43\x42\x31\x14\x30\x12\x06\x03\x55\x04\x0b\x13\x0b\x43\x43\x42\x5f\x4e\x65\x74\x62\x61\x6e\x6b\x31\x0b\x30\x09\x06\x03\x55\x04\x08\x13\x02\x42\x4a\x31\x14\x30\x12\x06\x03\x55\x04\x03\x13\x0b\x43\x43\x42\x20\x43\x41\x20\x52\x4f\x4f\x54\x31\x11\x30\x0f\x06\x03\x55\x04\x07\x13\x08\x58\x69\x20\x43\x68\x65\x6e\x67\x31\x23\x30\x21\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x09\x01\x16\x14\x63\x61\x2f\x7a\x68\x2f\x63\x63\x62\x40\x63\x63\x62\x2e\x63\x6f\x6d\x2e\x63\x6e\x30\x1e\x17\x0d\x39\x39\x30\x36\x32\x39\x30\x30\x30\x30\x30\x30\x5a\x17\x0d\x34\x39\x30\x36\x32\x39\x30\x30\x30\x30\x30\x30\x5a\x30\x81\x8c\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x43\x4e\x31\x0c\x30\x0a\x06\x03\x55\x04\x0a\x13\x03\x43\x43\x42\x31\x14\x30\x12\x06\x03\x55\x04\x0b\x13\x0b\x43\x43\x42\x5f\x4e\x65\x74\x62\x61\x6e\x6b\x31\x0b\x30\x09\x06\x03\x55\x04\x08\x13\x02\x42\x4a\x31\x14\x30\x12\x06\x03\x55\x04\x03\x13\x0b\x43\x43\x42\x20\x43\x41\x20\x52\x4f\x4f\x54\x31\x11\x30\x0f\x06\x03\x55\x04\x07\x13\x08\x58\x69\x20\x43\x68\x65\x6e\x67\x31\x23\x30\x21\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x09\x01\x16\x14\x63\x61\x2f\x7a\x68\x2f\x63\x63\x62\x40\x63\x63\x62\x2e\x63\x6f\x6d\x2e\x63\x6e\x30\x81\x9f\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x01\x05\x00\x03\x81\x8d\x00\x30\x81\x89\x02\x81\x81\x00\xa2\x3f\x25\x03\xf1\x32\x99\x9d\x84\x2f\xa2\xa8\x65\xe6\xdf\x59\x10\x2f\x6e\x58\xf8\x34\x14\xb7\x96\x45\xbd\x30\x11\x41\xce\x1a\xd0\x34\xdd\x3a\x17\xcf\xdf\xa3\x45\x5b\xe4\x43\xc4\x63\x64\x19\xc8\xee\xc6\x5f\xaa\x02\x71\xa1\x86\x38\x4b\x82\x4e\x4c\xa6\x40\xbe\xc1\x21\x28\x17\xdd\x5a\x9c\x55\x97\xa6\x10\x4f\x1e\x11\xbe\xb2\x52\x27\x41\x8b\xfb\xe2\x03\x91\x68\xb9\x9f\x72\x5c\x07\x7b\x5d\xf5\x0e\x00\x8f\x6b\x51\xed\x55\xc6\x90\xe4\x88\x58\x83\x3e\xc9\x8c\x0e\xa8\xec\x3b\xf2\xa5\x40\xe4\x7a\x94\xbf\x8e\xb1\x87\x02\x03\x01\x00\x01\xa3\x27\x30\x25\x30\x12\x06\x03\x55\x1d\x13\x01\x01\xff\x04\x08\x30\x06\x01\x01\xff\x02\x01\x02\x30\x0f\x06\x03\x55\x1d\x0f\x01\x01\xff\x04\x05\x03\x03\x00\x06\x00\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x04\x05\x00\x03\x81\x81\x00\x77\xee\xa7\xa7\xc1\xc5\x87\x9a\x52\xd6\x9e\x73\x7f\x2b\xa6\xa9\x9f\x3e\x67\xb3\xce\x58\x07\xb6\xe3\xef\x58\xee\x2c\xc3\x3c\xd7\x0a\xe1\x08\x6b\xf1\xe2\xc1\x63\x53\x27\x19\x15\xa5\xd1\xc5\xbc\x49\x44\xb1\xc9\x6b\xd2\x8f\x48\xd5\x60\xbb\x21\x48\xa1\xaf\xe9\xe6\x7f\xc3\x74\xca\x22\x92\x91\x4c\xff\xa7\x9a\x2a\x45\x13\xc6\x69\x54\x53\xd6\xf0\x43\xf1\xf6\xe6\x9c\xd1\xc7\xf9\x9d\xd6\x09\x8c\x90\x72\x1e\x33\x19\x37\x7e\xba\x04\x4d\x84\x10\x1b\x90\xe2\xe2\xf6\x63\x2d\xef\x40\xaf\xc8\x7b\xd3\x14\xe9\xb6\xd2\xe5\x17")
# define CCB_ROOT_CERT_LEN		699
#ifdef FF_EXE
#define CCB_ROOT_CERT_NICKNAME TEXT("CCB ROOT")
#define CCB_ROOT_CERT_NAME TEXT("ccbcert.cer")
#endif


#define ONKEY_CURRENT_REG_SLOT_ID		TEXT("CurrentRegSlotID")

#define ONKEY_SMARDCARD_SUPPORT			1  // 工程是否支持智能卡登录，1为支持，0为不支持


# define ONKEY_CUSTUMER_NAME			"CCB"
# define ONKEY_CUSTUMER_NAME_T			TEXT("CCB")

# define ONKEY_OEM_SUFFIX_NAME			TEXT("_") ONKEY_CUSTUMER_NAME_T
# define ONKEY_OEM_DEV_TYPE				TEXT("DB")

# define ONKEY_SERVICE_NAME				TEXT("OnKey Service CCB")


# define ONKEY_OEM_SUFFIX_NAME_A		"_"  ONKEY_CUSTUMER_NAME
# define ONKEY_OEM_SUFFIX_NAME			TEXT("_") ONKEY_CUSTUMER_NAME_T

#if(ONKEY_SMARDCARD_SUPPORT)
#define ONKEY_SMARTCARD_READER		"Tendyron Tendyron Token"//需要保证该读卡器名称是Tendyron
#endif

//-----------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
// 定制修改配置
#define ONKEY_LIB_EXT_NAME				TEXT(".a")
#ifdef __linux__
#define ONKEY_DLL_EXT_NAME				TEXT(".so")
#else
#define ONKEY_DLL_EXT_NAME				TEXT(".dylib")
#endif
#define ONKEY_EXE_EXT_NAME				TEXT("")

#define ONKEY_APP_OEM_CUSTOMID			"9336"	// 客户定制号
// 初始化密钥（16位）。如果没有初始化密钥，屏蔽下一行
#define ONKEY_APP_OEM_INITKEY			"\x0A\x8E\xEF\x01\xB9\x7E\x1F\x9F\xAB\xC6\xD1\x11\xED\x2E\x00\xD6"

# define ONKEY_CSP_PROV_NAME			"Tendyron OnKey CSP CCB v1.0"
# define ONKEY_TOKEN_LIBNAME_A			"D4Token_CCB.lib"

# define ONKEY_OEM_SUFFIX_NAME			TEXT("_CCB")
# define ONKEY_CSP_PROV_DLLNAME			TEXT("D4CSP")			ONKEY_OEM_SUFFIX_NAME	ONKEY_DLL_EXT_NAME
# define ONKEY_P11_DLLNAME				TEXT("libD4P11")		ONKEY_OEM_SUFFIX_NAME	ONKEY_DLL_EXT_NAME
# define ONKEY_TOKEN_LIBNAME			TEXT("D4Token")			ONKEY_OEM_SUFFIX_NAME	ONKEY_LIB_EXT_NAME
# define ONKEY_TOKEN_DLLNAME			TEXT("D4Token")			ONKEY_OEM_SUFFIX_NAME	ONKEY_DLL_EXT_NAME
# define ONKEY_DEVXX_DLLNAME			TEXT("libD4DevEx%.2d")	ONKEY_DLL_EXT_NAME
# define ONKEY_PINPAD_DLLNAME			TEXT("libD4Pinpad")		ONKEY_DLL_EXT_NAME
# define ONKEY_VD_DLLNAME				TEXT("D4VD")			ONKEY_OEM_SUFFIX_NAME	ONKEY_DLL_EXT_NAME
# define ONKEY_SVR_REGCERTEXE			TEXT("D4Svr")			ONKEY_OEM_SUFFIX_NAME	ONKEY_EXE_EXT_NAME

# define ONKEY_MON_NAME					TEXT("D4MON")			ONKEY_OEM_SUFFIX_NAME	ONKEY_EXE_EXT_NAME	

# define ONKEY_INSIDE_REGPATH			TEXT("Software\\Tendyron\\193D4") ONKEY_OEM_SUFFIX_NAME

// --------------------------------------------------------------------------------------------

#define CRITICAL_SIGN_TYPE						1  // 1 - XML解析
												   // 2 - TLV解析

#define CRITICAL_SIGN_TYPE_XML					1
#define CRITICAL_SIGN_TYPE_TLV					2
//ini读取
enum{
	ADD_NONE = 1,
	ADD_PRON,
	ADD_PRON_DWORD,
	ADD_DWORD_PRON,
	ADD_STRDD_PRON,
	ADD_PRON_STRDD,
	ADD_CLTN,
	ADD_CLTN_DWORD,
	ADD_DWORD_CLTN,
	ADD_STRDD_CLTN,
	ADD_CLTN_STRDD,
	ADD_PRON_CLTN,
	ADD_CLTN_PRON,
	ADD_PRON_CLTN_DWORD,
	ADD_PRON_CLTN_STRDD,
	ADD_CLTN_PRON_DWORD,
	ADD_CLTN_PRON_STRDD,
	ADD_DWORD_CLTN_PRON,
	ADD_DWORD_PRON_CLTN,
	ADD_STRDD_CLTN_PRON,
	ADD_STRDD_PRON_CLTN,
	ADD_CLTN_DWORD_PRON,
	ADD_CLTN_STRDD_PRON,
	ADD_PRON_DWORD_CLTN,
	ADD_PRON_STRDD_CLTN,
	ADD_PRON_PRON_PRON,
	ADD_PRON_PRON,
	ADD_DWORD_PRON_PRON_PRON, //剩余次数+网银盾+网银盾+网银盾
};

#define ONKEY_INIDEF_TEXT					TEXT("TEXT")
#define ONKEY_INIDEF_IDDTEXT				TEXT("IDD_TEXT")
#define ONKEY_INIDEF_COMMONINFO				TEXT("COMMON_INFO")
#define ONKEY_INIDEF_KEYWORDS				TEXT("KEYWORDS")

#define ONEKY_INIDEF_KEYWORDS_CLIENTNAME	TEXT("ClientName")
#define ONKEY_INIDEF_KEYWORDS_PRODUCTION	TEXT("Production")

#define ONKEY_INIDEF_MAXBUFLEN				256
//-----------------------------------------------------------------------------------------------------
// 广播消息名称
#define ONKEY_WM_BROADCAST_MESSAGE_NAME  TEXT("{059D1DCF-7032-4eb5-A58B-33E72F639DC2}_") ONKEY_CUSTUMER_NAME_T
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
// 兼容性配置
#define ONKEY_SUPPORT_OLD_CONTAINERNAME         0   // 只有浦发的版本需要此定义
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// 虚拟桌面+软键盘配置
#define ONKEY_SUPPORT_VIRTUALDESTTOP_SOFTPAD	0		// 虚拟桌面+软键盘
#define ONKEY_KEYBOAD_AND_PINPAD_INPUT			1		// 键盘和软键盘同时输入密码
#define ONKEY_PINPAD_KEY_POS_RANDOM				1		// 软件键盘键值随机变化
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// 以下定义只限于指定工程
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
// OnKeyReg 配置
#ifdef _ONKEYREG_PROJECT_
#define ONKEY_REG_SUPPORT_TRAYICON				0	// 使用系统托盘提示信息
#define ONKEY_REG_SUPPORT_DEVTYP_VOLUME			1	// 监控U盘拔插
#define ONKEY_REG_SUPPORT_DEVNODES_CHANGED		1	// win2k非sp4需要定义为1
#define ONKEY_INSTALL_ROOCERT
#endif
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// CSP 配置
#ifdef _ONKEYCSP_PROJECT_
// 在接口写入证书，启动证书管理管理（删除同CN早期的证书）
#define ONKEY_CSP_SUPPORT_CERTMANAGE			1	// 目前只有深发展使用此宏

#define ONKEY_CSP_SUPPORT_CERT_ADDTOSYS			0	// 所有注册证书的工作现在都由P11完成
#define ONKEY_CSP_SUPPORT_GENKEY_TIP_ON			1	// 产生密钥对提示信息
#endif
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// OnKeyP11 配置
// P11接口写入的证书注册到IE上，为了测试
#define ONKEY_P11_SUPPORT_CERT_ADDTOSYS			1
#define ONKEY_P11_CODE_SUPPORT_DEV_ALG			1	// 是否支持ssf33 scb2算法
#define ONKEY_P11_SUPPORT_GENKEY_OVERWRITE		1	// 密钥对满时，生成密钥对提示覆盖对话框
#define  ONKEY_P11_SUPPORT_CERTMANAGE_BYSN		0	//CCB 删除证书通过CN，0为全部删除，CCB特殊需求
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// Token 层的配置
#ifdef _ONKEYTOKEN_PROJECT_
#define ONKEY_SUPPORT_SHAREPIN					0	// 配置是否支持 进程之间共享PIN
#define ONKEY_SUPPORT_PIN_TIMEOUT				0	// 配置是否支持 PIN超时
#define ONKEY_SUPPORT_CLOSE_DEVICE				1	// 配置是否支持 每次指令流操作后关闭设备句柄
#endif
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// Device 层的配置
#ifdef _ONKEYDEV_PROJECT_
#define ONKEY_DEV_CODE_SUPPORT_HIP_INPUTPIN		1	// 启用HIP方式输入密码代码
#define ONKEY_DEV_CODE_SUPPORT_BUTTONSIGN		1	// 启用HIP签名代码
#define ONKEY_DEV_CODE_SUPPORT_VOICESIGN		1	// 启用HIP及语音签名代码
#define ONKEY_DEV_CODE_SUPPORT_DISPLAYSIGN		1	// 启用HIP及显示签名代码
#define ONKEY_DEV_193V_MINUSERPINLEN			6	// 最小密码长度，193V产品没有在KEY保存该字段
#define ONKEY_DEV_193V_MAXUSERPINLEN			8	// 最大密码长度
#endif
//-----------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------
// PinPad层对话框按钮颜色配置
// 按钮颜色
#ifdef _ONKEYPINPAD_PROJECT_//
#define ONKEY_MOVIE_USE_BMP						0		//1-使用bmp做动画，0-使用gif做动画
#define ONKEY_VIRTUALDESKTOP_SAVE_WALLPAPER		1		// 虚拟桌面是否支持保存桌面壁纸
#if (ONKEY_PINPAD_KEY_POS_RANDOM)
#define ONKEY_PINPAD_NUM_RAND_TIME				80
#define ONKEY_PINPAD_CHAR_RAND_TIME				150
#endif
//const DWORD CLR_Dialog		= RGB(222, 235, 247);
//const DWORD CLR_Normal		= RGB(180, 200, 150);//RGB(189, /*219*/, 165);
//const DWORD CLR_Selected	= RGB(112, 187, 109);
//const DWORD CLR_Focus		= RGB(180, 200, 150);
//const DWORD CLR_Hover		= RGB(112, 187, 1);

const DWORD CLR_CCB_Key		= RGB( 7, 91, 195);

const DWORD CLR_Dialog		= RGB(236, 233, 216);
const DWORD CLR_Normal		= RGB(236, 233, 216);//RGB(189, /*219*/, 165);
const DWORD CLR_Selected	= RGB(192, 192, 192);//192, 192, 192 
const DWORD CLR_Focus		= RGB(192, 192, 192);//
const DWORD CLR_Hover		= RGB(255, 255, 255);//236, 233, 216
const DWORD CLR_Disabled	= GetSysColor(COLOR_BTNFACE);// 
const DWORD CLR_Text		= RGB(0,0,0);
#endif
//-----------------------------------------------------------------------------------------------------

//删除单个证书
#define KP_DELETE_CERT		0xB0



//-----------------------------------------------------------------------------------------------------
// end
//-----------------------------------------------------------------------------------------------------

#endif