//----------------
// 音频文件类 头文件
// 2015.1.4
// Fairchild_Lhy
//----------------
#pragma once 
#include "stdafx.h"
#include <assert.h>

// 非MFC工程需要包含的头文件
// #include <string.h>

// 参考文献：
// 1、ID3v2标签    开发者文档 http://id3.org/d3v2.3.0
// 2、APEv2标签    开发者文档 http://wiki.hydrogenaud.io/index.php?title=APEv2_specification
// 3、FLAC格式	  开发者文档 https://xiph.org/flac/format.html#def_STREAMINFO
// 4、FLAC标签     开发者文档 http://www.xiph.org/vorbis/doc/v-comment.html

// 释放指针的模版
template<class T> void SingleDelete(T & t) {
	if(t){
		delete t;
		t = NULL;
	}
}
template<class T> void MultiDelete(T & t) {
	if(t){
		delete []t;
		t = NULL;
	}
}

// 函数原型声明
void DropSpace(CString & cstr);
void UnicodeToAnsi(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert);
void UnicodeToUTF8(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert);
void UintTo4BytesUCharBigEndian(const UINT & uint, UCHAR uchar[4]);
void UintTo3BytesUCharBigEndian(const UINT & uint, UCHAR uchar[3]);

//----------AudioFile类相关的定义----------
// 文件类型
enum FileType {
	fileType_unknown, // 未知文件
	fileType_mp3,     // MP3文件
	fileType_ape,     // APE文件
	fileType_flac,    // FLAC文件
	fileType_wav,     // WAV文件
	fileType_png,     // PNG文件
	fileType_jpg_jpeg // JPG,JPEG文件
};

#define TASK_TYPE_COUNT 5 // 音频任务种类，序号与enum FileType一致

// 文件类型数目
#define _FILE_TYPE_COUNT 7

// 文件后缀名最大长度（单位：字）
#define _MAX_FILE_TYPE_LENGTH 6

// 音频文件标签信息最大长度（单位：字）
#define _MAX_TITLE_LENGTH  50
#define _MAX_ARTIST_LENGTH 50
#define _MAX_ALBUM_LENGTH  50
#define _MAX_STYLE_LENGTH  50
#define _MAX_YEAR_LENGTH   10

// 文件信息结构
struct FileInfo {
	wchar_t chFilePath[_MAX_PATH];			   // 文件路径
	wchar_t chFileName[_MAX_PATH];			   // 文件名
	wchar_t chFileType[_MAX_FILE_TYPE_LENGTH]; // 文件后缀名
	UINT    nFileType;						   // 后缀名对应编号
};

// 音频文件标签信息结构
struct TagInfo {
	wchar_t chTitle[_MAX_TITLE_LENGTH];   // 标题
	wchar_t chArtist[_MAX_ARTIST_LENGTH]; // 艺术家
	wchar_t chAlbum[_MAX_ALBUM_LENGTH];   // 专辑
	wchar_t chStyle[_MAX_STYLE_LENGTH];   // 流派
	wchar_t chYear[_MAX_YEAR_LENGTH];	  // 年代
};

//----------Mp3Tag类相关的定义----------
// ID3v2标签帧类型
enum ID3v2FrameType {
	id3v2FrameType_title,   // 标题帧
	id3v2FrameType_artist,  // 艺术家帧
	id3v2FrameType_album,   // 专辑帧
	id3v2FrameType_year,    // 年代帧
	id3v2FrameType_style,   // 流派帧
	id3v2FrameType_comment, // 注释帧
	id3v2FrameType_picture  // 图片帧
};

// ID3v2帧类型数量
#define _ID3V2_FRAME_TYPE_COUNT 7

// ID3v1标签各项信息长度及总长度（单位：字节）
#define _ID3V1_IDENTIFIER_LENGTH 3
#define _ID3V1_YEAR_LENGTH       4
#define _ID3V1_TITLE_LENGTH		 30
#define _ID3V1_ARTIST_LENGTH	 30
#define _ID3V1_ALBUM_LENGTH		 30
#define _ID3V1_COMMENT_LENGTH	 28
#define _ID3V1_LENGTH			 128

// ID3v1标签从MP3文件尾部的偏移量（单位：字节）
#define _ID3V1_OFFSET_FROM_END -_ID3V1_LENGTH

// ID3v2标签头各项信息长度及总长度（单位：字节）
#define _ID3V2_TAG_HEADER_IDENTIFIER_LENGTH	3
#define _ID3V2_TAG_HEADER_VERSION_LENGTH    2
#define _ID3V2_TAG_HEADER_TAG_SIZE_LENGTH   4
#define _ID3V2_TAG_HEADER_LENGTH			10

// ID3v2帧头各项信息长度及总长度（单位：字节）
#define _ID3V2_FRAME_HEADER_ID_LENGTH         4
#define _ID3V2_FRAME_HEADER_FRAME_SIZE_LENGTH 4
#define _ID3V2_FRAME_HEADER_FLAGS_LENGTH      2
#define _ID3V2_FRAME_HEADER_LENGTH			  10

// ID3v2帧内容各项信息长度（单位：字节）
#define _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH 2
#define _ID3V2_FRAME_BODY_BODY_LENGTH		    50

// ID3v2注释帧各项信息长度（单位：字节）
#define _ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH 3  
#define _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH  4 

// ID3v2图片帧结构各项信息长度（单位：字节）
#define _ID3V2_PICTURE_FRAME_DESCRIP_LENGTH _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH

// ID3v1标签标识符
extern const char _ID3V1_IDENTIFIER[];

// ID3v2标签头标识符
extern const char _ID3V2_IDENTIFIER[];

// ID3v1标签默认注释信息
extern const char* _ID3V1_DEFAULT_COMMENT;

// ID3v2标签默认注释信息
extern const wchar_t* _ID3V2_DEFAULT_COMMENT;

// ID3v1标签不使用Genre信息
#define _ID3V1_GENRE_NOT_USE 0xFF

// ID3v2标签版本号
extern const char _ID3V2_VERSION[];

// ID3v2帧内容使用Unicode编码填写时需要添加的用于辨识的前缀
#define _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE 0x01

// ID3v2帧内容使用Unicode编码写入之前需要添加的前缀
extern const UCHAR _ID3V2_FRAME_USE_UNICODE_PREFIX[];

// ID3v2帧头标识ID种类，序号要与enum ID3v2FrameType对应
extern const char _ID3v2_FRAME_HEADER_ID_TYPES[][_ID3V2_FRAME_HEADER_ID_LENGTH];

// ID3v2注释帧内容使用的语种
extern const char _ID3V2_COMMENT_FRAME_LANGUAGE_CHINESE[];

// ID3v2注释帧默认描述
extern const UCHAR _ID3V2_DEFAULT_COMMENT_DESCRIP[];

// ID3v2图片帧默认描述
#define _ID3V2_DEFAULT_PICTURE_DESCRIP _ID3V2_DEFAULT_COMMENT_DESCRIP

// ID3v2图片帧使用的MIME类型对应的代码，顺序号要与enum fileType里的顺序一致，前面用空字符串占位
extern const char* _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE[];

// ID3v2图片帧使用的MIME类型对应的代码字符串长度，包含结尾符0x00（单位：字节），顺序号要与enum fileType里的顺序一致，前面用0占位
extern const UINT _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE_LENGTH[];

// ID3v2图片帧使用的图片类型描述字节（封面front）
#define _ID3V2_PICTURE_FRAME_PICTURE_TYPE_COVER_FRONT 0x03

// ID3v1标签结构
struct ID3v1 {
	char  chIdentifier[_ID3V1_IDENTIFIER_LENGTH]; // 标识符
	char  chTitle[_ID3V1_TITLE_LENGTH];			  // 标题帧
	char  chArtist[_ID3V1_ARTIST_LENGTH];		  // 艺术家帧
	char  chAlbum[_ID3V1_ALBUM_LENGTH];			  // 专辑帧
	char  chYear[_ID3V1_YEAR_LENGTH];			  // 年代帧
	char  chComment[_ID3V1_COMMENT_LENGTH];		  // 注释帧
	char  chReserved;						      // 保留位
	char  chTrack;							      // 音轨，歌曲在专辑中的曲目号
	UCHAR chGenre;							      // 流派编号，由于ID3v1现已不常用，所以不使用此信息。编号对照表见 http://baike.baidu.com/view/66078.htm
												  // 定义为UCHAR，防止赋初值（0xFF，表示不使用）时报截断警告
};

// ID3v2标签头结构
struct ID3v2TagHeader {
	char  chIdentifier[_ID3V2_TAG_HEADER_IDENTIFIER_LENGTH]; // 标识符，默认是“ID3”
	char  chVersion[_ID3V2_TAG_HEADER_VERSION_LENGTH];		 // 版本号
	char  chFlag;											 // 标志位，一般不使用，置0即可
	UCHAR chTagSize[_ID3V2_TAG_HEADER_TAG_SIZE_LENGTH];		 // ID3v2标签总大小，不含本标签头的10字节，定义为UCHAR，防止赋值时报截断警告
};

// ID3v2标签帧帧头结构
struct ID3v2FrameHeader {
	char  chFrameID[_ID3V2_FRAME_HEADER_ID_LENGTH];			  // 帧类型标识，对照表参见 http://id3.org/d3v2.3.0 第4.2.1节
	UCHAR chFrameSize[_ID3V2_FRAME_HEADER_FRAME_SIZE_LENGTH]; // 帧内容长度，不含帧头10字节，但要含上帧类型标识，和Unicode前缀，共3字节,定义为UCHAR，防止赋值时报截断警告
	char  chFlags[_ID3V2_FRAME_HEADER_FLAGS_LENGTH];		  // 帧内容标识符，一般不使用，默认为0x00，0x00
};

// ID3v2标签帧内容结构
struct ID3v2FrameBody {
	char    chEncode;												  // 帧内容编码方式，默认使用Unicode宽字符编码，此字符置为0x01
	UCHAR   chUnicodePrefix[_ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH]; // 帧内容使用Unicode时，需要在字符串前加的前缀，定义为UCHAR，防止赋值时报截断警告
	wchar_t chBody[_ID3V2_FRAME_BODY_BODY_LENGTH];					  // 帧内容
	UINT    nBodyLengthInBytes;										  // 帧内容长度（不含编码方式和Unicode前缀）
};

// ID3v2标签帧结构
struct ID3v2Frame {
	ID3v2FrameHeader header;		 // 帧头
	ID3v2FrameBody   body;			 // 帧内容
};

// ID3v2注释帧结构，与普通帧相比较特殊。
struct ID3v2CommentFrame {
	ID3v2FrameHeader header;										           // 帧头
	char             chTextEncoding;										   // 内容编码方式
	char			 chLanguage[_ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH];		   // 内容使用的语种 中文是chi 参考ISO-639-2规范 http://baike.baidu.com/view/3351971.htm
	UCHAR			 chContentDescrip[_ID3V2_COMMENT_FRAME_DESCRIP_LENGTH];	   // 对注释的描述，可变长度，但我们不需要，所以使用空UNICODE字符串FF FE 00 00
	UCHAR			 chUnicodePrefix[_ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH]; // 帧内容使用Unicode时，需要在字符串前加的前缀，定义为UCHAR，防止赋值时报截断警告
	wchar_t			 chBody[_ID3V2_FRAME_BODY_BODY_LENGTH];					   // 注释内容	
	UINT			 nBodyLengthInBytes;									   // 注释内容长度
};

// ID3v2图片帧结构，与普通帧相比较特殊。
struct ID3v2PictureFrame {
	ID3v2FrameHeader header;										 // 帧头
	char             chTextEncoding;								 // 内容编码方式
	const char*  	 chMIMEType;									 // 图片格式对应的MIME代码，为字符串，需要带上结尾符0x00
	UINT             nMIMETypeLengthInBytes;						 // 图片格式对应的MIME代码的长度，需要算上结尾字符串占的1个字节
	char			 chPictureType;									 // 图片类型描述，我们使用0x03表示CD封面图 更多类型参考 http://id3.org/d3v2.3.0 4.15章节
	UCHAR			 chDescrip[_ID3V2_PICTURE_FRAME_DESCRIP_LENGTH]; // 对图片的描述，我们不使用描述，故使用空的Unicode字符串 0xFF,0xFE,0x00,0x00
	char*			 chPictureData;									 // 图片数据
	UINT			 nPicDataLengthInBytes;							 // 图片数据的总长度
};

// ID3v2标签结构
struct ID3v2 {
	ID3v2TagHeader    id3v2TagHeader;    // 标签头 
	ID3v2Frame	      id3v2FrameTitle;   // 标题帧
	ID3v2Frame	      id3v2FrameArtist;  // 艺术家帧
	ID3v2Frame	      id3v2FrameAlbum;   // 专辑帧
	ID3v2Frame	      id3v2FrameYear;    // 年代帧
	ID3v2Frame	      id3v2FrameStyle;   // 流派帧
	ID3v2CommentFrame id3v2FrameComment; // 注释帧
	ID3v2PictureFrame id3v2FramePicture; // 图片帧
};

//----------ApeTag类相关的定义----------
// APEv2ItemKey类型
enum APEv2ItemKeyType {
	apev2ItemKeyType_title,  // 标题
	apev2ItemKeyType_artist, // 艺术家
	apev2ItemKeyType_album,  // 专辑
	apev2ItemKeyType_year,	 // 年代
	apev2ItemKeyType_style,  // 流派 
	apev2ItemKeyType_comment // 注释
};

// APEv2ItemKey类型数量
#define _APEV2_ITEM_KEY_TYPE_COUNT 6

// APEv2标签头/尾各项信息长度及总长度（单位：字节）
#define _APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH 8
#define _APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH	   4
#define _APEV2_TAG_HEADER_FOOTER_RESERVED_LENGTH   8
#define _APEV2_TAG_HEADER_FOOTER_LENGTH			   32

// APEv2Item结构各信息长度（单位：字节 ）
#define _APEV2_ITEM_FLAGS_LENGTH 4
#define _APEV2_ITEM_BODY_LENGTH  200 // 内容编码：UTF8

// APEv2标签版本号2.0
extern const UINT _APEV2_TAG_VERSION_2_0;

// APEv2标签默认注释信息
#define _APEV2_DEFAULT_COMMENT _ID3V2_DEFAULT_COMMENT

// APEv2标签头/尾标识，详细设置参考 http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags
extern const UCHAR _APEV2_TAG_FLAGS_HEADER[];
extern const UCHAR _APEV2_TAG_FLAGS_FOOTER[];

// APEv2标签标识符
extern const char _APEV2_TAG_IDENTIFIER[];

// APEv2ItemKey各类型对应的标识，序号要与enum APEv2ItemKeyType对应
extern const char* _APEV2_ITEM_KEY[];

// APEv2ItemKey各类型对应的标识的长度，含字符串尾的0x00字节，序号要与enum APEv2ItemKeyType对应
extern const UINT _APEV2_ITEM_KEY_LENGTH[];

// APEv2标签头/尾结构
struct APEv2TagHeaderFooter {
	char  chIdentifier[_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH]; // 标识符 "APETAGEX"
	UINT  nVersion;											        // 版本号 整形值2000代表Ver2.0
	UINT  nTagSize;												    // 标签总大小，单位：字节，不含标签头，含标签尾+所有Items
	UINT  nItemCount;											    // Item个数
	UCHAR chFlags[_APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH];			// 标识本标签是标签头还是标签尾等信息			
	char  chReserved[_APEV2_TAG_HEADER_FOOTER_RESERVED_LENGTH];		// 保留位，全置0
};
typedef APEv2TagHeaderFooter APEv2TagHeader;
typedef APEv2TagHeaderFooter APEv2TagFooter;

// APEv2Item结构
struct APEv2Item {
	UINT        nBodyLengthInBytes;				   // Item内容的长度
	char	    chFlags[_APEV2_ITEM_FLAGS_LENGTH]; // 标识符，常用全0，详细参考 http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags 
	const char* pKey;							   // Item类型
	UINT        nKeyLengthInBytes;				   // Item类型长度，含字符串结尾0x00的一个字节
	char        chBody[_APEV2_ITEM_BODY_LENGTH];   // Item内容 UTF8编码
};

// APEv2标签结构
struct APEv2 {
	APEv2TagHeader apev2TagHeader;   // 标签头
	APEv2Item	   apev2ItemTitle;   // 标题
	APEv2Item	   apev2ItemArtist;  // 艺术家
	APEv2Item	   apev2ItemAlbum;   // 专辑
	APEv2Item	   apev2ItemYear;    // 年代
	APEv2Item	   apev2ItemStyle;   // 流派
	APEv2Item	   apev2ItemComment; // 注释
	APEv2TagFooter apev2TagFooter;   // 标签尾
};

//----------FlacTag类相关的定义----------
// FLAC文件Block类型，官方规定按以下固定顺序，不可改变。整型7-126未定义，127非法。
enum FLACBlockType {
	flacBlockType_streamInfo,    // 0,包含整个比特流的一些信息，如采样率、声道数、采样总数等。它一定是第一个block而且必须有。之后可以接其他block，这些block可以不用识别直接跳过
	flacBlockType_padding,       // 1,保留块，用于后期添加，一般作为文件中最后一个块，本软件规定Padding必须存在且作为最后一个块
	flacBlockType_application,   // 2,包含第三方应用软件信息，这个段里的32位识别码是flac维护组织提供的，是唯一的
	flacBlockType_seekTable,     // 3,保存快速定位点，一个点由18bytes组成（2k就可以精确到1%的定位），表里可以有任意多个定位点
	flacBlockType_vorbisComment, // 4,存储了一系列可读的“名/值”的键值对，使用UTF-8编码。这是flac唯一官方支持的标签段
	flacBlockType_cueSheet,      // 5,存储用在cue sheet中的各种信息。可以用来划分音轨，在备份CD时十分有用
	flacBlockType_picture		 // 6,保存相关图片，同时还可以有url、分辨率等信息，可以有不止一个picture block
};

// FLAC文件Block类型数目
#define _FLAC_BLOCK_TYPE_COUNT 7

// FLAC标签帧类型
enum FLACFrameType {
	flacFrameType_title,   // 标题帧
	flacFrameType_artist,  // 艺术家帧
	flacFrameType_album,   // 专辑帧
	flacFrameType_year,    // 年代帧
	flacFrameType_style,   // 流派帧
	flacFrameType_comment  // 注释帧
};

// FLAC标签帧类型数目
#define _FLAC_FRAME_TYPE_COUNT 6

// FLAC文件头部标识长度（单位：字节）
#define _FLAC_IDENTIFIER_LENGTH 4

// FLAC文件Block头结构的各项信息长度及总长度（单位：字节）
#define _FLAC_BLOCK_HEADER_SIZE_LENGTH 3
#define _FLAC_BLOCK_HEADER_LENGTH      4

// FLAC文件默认Paddind长度（单位：字节），整个块必须是8的倍数，块头4字节，内容使用1KB - 4，即1020Bytes。
#define _FLAC_DEFAULT_PADDING_LENGTH 1020

// FLAC文件标签帧结构各项信息长度（单位：字节）
#define _FLAC_FRAME_SIZE_LENGTH 4

// FLAC文件标签块默认描述长度（单位：字节）
#define _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH 29

// FLAC文件标签块最大帧内容长度（单位：字节）
#define _FLAC_FRAME_BODY_LENGTH 200

// FLAC文件标签块除开各个标签的基础长度（单位：字节）
#define _FLAC_BLOCK_VORBIS_COMMENT_BASE_LENGTH 37

// FLAC文件图片块各项信息长度（单位：字节）
#define _FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH         4
#define _FLAC_BLOCK_PICTURE_MIME_SIZE_LENGTH        4
#define _FLAC_BLOCK_PICTURE_DESCRIP_SIZE_LENGTH     4
#define _FLAC_BLOCK_PICTURE_PIC_WIDTH_LENGTH        4
#define _FLAC_BLOCK_PICTURE_PIC_HEIGHT_LENGTH       4
#define _FLAC_BLOCK_PICTURE_COLOR_DEPTH_LENGTH      4
#define _FLAC_BLOCK_PICTURE_INDEX_COLOR_USED_LENGTH 4
#define _FLAC_BLOCK_PICTURE_PIC_DATA_SIZE_LENGTH    4

// FLAC文件图片块基础长度（单位：字节）
#define _FLAC_BLOCK_PICTURE_BASE_LENGTH 32

// FLAC文件头部标识（每个FLAC文件一开始都必须有）
extern const char* _FLAC_IDENTIFIER;

// FLAC文件中最后一个块的判断掩码
extern const UCHAR _FLAC_IS_LAST_BLOCK_MASK;

// FLAC文件取出块类型的掩码
extern const UCHAR _FLAC_GET_BLOCK_TYPE_MASK;

// FLAC文件Padding块作为最后一个块的Block类型字节
extern const UCHAR _FLAC_PADDING_AS_LAST_BLOCK;

// FLAC文件标签块类型字节
extern const UCHAR _FLAC_BLOCK_VORBIS_COMMENT_TYPE;

// FLAC文件图片块类型字节
extern const UCHAR _FLAC_BLOCK_PICTURE_TYPE;

// FLAC文件默认Paddind大小，整个块必须是8的倍数，块头4字节，内容使用1KB - 4，FLAC规定大端存放即00 03 FC
extern const UCHAR _FLAC_DEFAULT_PADDING_SIZE[];

// FLAC文件标签块默认描述
extern const char* _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP;

// FLAC文件标签块注释帧默认注释信息
#define _FLAC_DEFAULT_COMMENT _ID3V2_DEFAULT_COMMENT

// FLAC文件图片块使用的图片类型描述字节（封面front）
extern const char _FLAC_BLOCK_PICTURE_PIC_TYPE_COVER_FRONT[];

// FLAC文件图片块使用的MIME类型对应的代码，顺序号要与enum fileType里的顺序一致，前面用空字符串占位
#define _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE

// FLAC文件图片块使用的MIME类型对应的代码字符串长度，不含结尾符0x00（单位：字节），顺序号要与enum fileType里的顺序一致，前面用0占位
extern const UINT _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE_LENGTH[];

// FLAC文件标签块帧类型标识，顺序要与enum FLACFrameType一致
extern const char* _FLAC_FRAME_TYPE_IDENTIFIER[];

// FLAC文件Block头结构
struct FLACBlockHeader {
	UCHAR chType;								  // 块信息类型，最高位指示此块是不是本FLAC文件中最后一个块
	UCHAR chSize[_FLAC_BLOCK_HEADER_SIZE_LENGTH]; // 块内容大小 
};

// FLAC文件普通Block内容结构
struct FLACBlockBody {
	char*			 chBody;			 // 块内容
	UINT			 nBodyLengthInBytes; // 块内容总长度
};

// FLAC文件普通Block结构（由于我们不关心，故只记录大小即可）
struct FLACBlock {
	FLACBlockHeader header; // 块头
	FLACBlockBody	body;   // 块内容
};

// FLAC文件标签帧结构
struct FLACFrame {
	UINT nBodyInBytes;					  // 帧内容长度
	char chBody[_FLAC_FRAME_BODY_LENGTH]; // 帧内容
};

// FLAC文件标签块结构
struct FLACVorbisComment {
	FLACBlockHeader header;													// 块头
	UINT            nDescripLengthInBytes;									// 标签块描述长度
	char			chDescrip[_FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH]; // 标签块描述，UTF-8编码，对于英文，和Ansi编码一致
	UINT			nFrameCount;											// 帧总个数
	FLACFrame       title;													// 标题帧
	FLACFrame       artist;													// 艺术家帧
	FLACFrame       album;													// 专辑帧
	FLACFrame       year;													// 年代帧
	FLACFrame       style;													// 流派帧
	FLACFrame       comment;												// 注释帧
};

// FLAC文件图片块结构
struct FLACPicture {
	FLACBlockHeader header;													   // 块头
	char		chPicType[_FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH];				   // 图片类型描述，我们使用0x00 0x00 0x00 0x03表示CD封面图 更多类型参考 http://id3.org/d3v2.3.0 4.15章节
	UCHAR		chMIMESize[_FLAC_BLOCK_PICTURE_MIME_SIZE_LENGTH];			   // MIME类型符大小，“大端式”存放
	const char* chMIMEType;													   // MIME类型符
	UINT		nMIMETypeLengthInBytes;										   // MIME类型符长度
	char		chDescripSize[_FLAC_BLOCK_PICTURE_DESCRIP_SIZE_LENGTH];		   // 描述图片的UTF-8字符串大小，我们默认不描述，所以为0
	char*	    chDescrip;													   // 描述图片的字符串，我们暂不使用
	char		chPicWidth[_FLAC_BLOCK_PICTURE_PIC_WIDTH_LENGTH];			   // 图片宽度，单位像素，“大端式”存放，涉及到对图片格式的读取，暂时不管，置0即可
	char		chPicHeight[_FLAC_BLOCK_PICTURE_PIC_HEIGHT_LENGTH];			   // 图片高度，单位像素，“大端式”存放，同上
	char		chColorDepth[_FLAC_BLOCK_PICTURE_COLOR_DEPTH_LENGTH];		   // 图片颜色深度，涉及到对图片格式的读取，暂时不管，置0即可
	char		chIndexColorUsed[_FLAC_BLOCK_PICTURE_INDEX_COLOR_USED_LENGTH]; // 使用颜色索引的数量，涉及到对图片格式的读取，暂时不管，置0即可
	UCHAR	    chPicDataSize[_FLAC_BLOCK_PICTURE_PIC_DATA_SIZE_LENGTH];	   // 图片数据大小，“大端式”存放
	char*	    chPicData;													   // 图片数据
	UINT		nPicDataLengthInBytes;										   // 图片数据长度
};

// FLAC文件头结构
struct FLAC {
	char			  chIdentifier[_FLAC_IDENTIFIER_LENGTH]; // FLAC文件头标识符
	FLACBlock		  BlockList[_FLAC_BLOCK_TYPE_COUNT];     // Block列表，只存放我们不关心的普通Block的信息
	FLACVorbisComment vorbisComment;						 // FLAC标签
	FLACPicture		  picture;								 // 封面图片
};


class AudioFile {
private:
	//-----成员变量-----
	FileInfo	 audioInfo; // 音频文件信息
	TagInfo		 tagInfo;   // 音频文件标签信息
	FileInfo     picInfo;   // 音频关联封面图文件信息

public:
	//-----成员函数（公有）-----
	AudioFile();									   // 默认构造函数
	~AudioFile();									   // 析构函数
	explicit AudioFile(const AudioFile & audioFile);   // 复制构造函数
	AudioFile& operator=(const AudioFile & audioFile); // 重载赋值操作符
	BOOL Open(const wchar_t* filePath);				   // 打开音频文件，从文件名中读取出标签信息
	BOOL AddCoverPicture(const wchar_t* picPath);	   // 添加封面图片
	const FileInfo& GetAudioInfo();					   // 获取音频文件信息
	const FileInfo& GetPicInfo();					   // 获取封面图片文件信息
	const TagInfo&  GetTagInfo();					   // 获取音频文件标签信息
	void  StartEdit();								   // 开始修改标签信息

private:
	//-----内在实现-----
	void GenerateFileInfo(const wchar_t* filePath, FileInfo & info); // 根据文件完整路径生成文件信息结构
	void ReadTagInfoFromFileName();								     // 从文件名读取标签信息

private:
	void* operator new(size_t size){return malloc(size);} // 重载new操作符，禁止生成堆对象，用以提高程序效率，减低内存管理压力
	void  operator delete(void* pp) { free(pp); }		  // 对称的，重载delete操作符，禁止生成堆对象
};
//-----函数实现（内联）-----
inline		           AudioFile::AudioFile(){
	memset(&tagInfo, 0, sizeof(tagInfo));	  // 初始化音频文件信息
	memset(&audioInfo, 0, sizeof(audioInfo)); // 初始化音频文件标签信息
	memset(&picInfo, 0, sizeof(picInfo));	  // 初始化音频关联封面图文件信息
}
inline				   AudioFile::~AudioFile() { 
	return; 
}
inline				   AudioFile::AudioFile(const AudioFile & audioFile) 
	: audioInfo(audioFile.audioInfo), tagInfo(audioFile.tagInfo), picInfo(audioFile.picInfo){
	return;
}
inline AudioFile&	   AudioFile::operator=(const AudioFile & audioFile) {
	this->audioInfo = audioFile.audioInfo;
	this->tagInfo = audioFile.tagInfo;
	this->picInfo = audioFile.picInfo;
	return *this;
}
inline const FileInfo& AudioFile::GetAudioInfo() {
	// 检测是否已经拥有合法的音频文件信息
	assert(PathFileExists(audioInfo.chFilePath)); // 如果没有对应路径的文件则报错，终止程序
	assert(wcslen(audioInfo.chFileName));         // 如果文件名长度为0则报错，终止程序
	assert(wcslen(audioInfo.chFileName));		  // 如果后缀名长度为0则报错，终止程序
	assert(audioInfo.nFileType != type_unknown);  // 如果后缀名不可识别则报错，终止程序

	return audioInfo;
}
inline const FileInfo& AudioFile::GetPicInfo() {
	// 检测是否已经拥有合法的音频文件信息
	//assert(PathFileExists(picInfo.chFilePath)); // 如果没有对应路径的文件则报错，终止程序
	//assert(wcslen(picInfo.chFileName));         // 如果文件名长度为0则报错，终止程序
	//assert(wcslen(picInfo.chFileName));		    // 如果后缀名长度为0则报错，终止程序
	//assert(picInfo.nFileType != type_unknown);  // 如果后缀名不可识别则报错，终止程序

	return picInfo;
}
inline const TagInfo&  AudioFile::GetTagInfo() {
	return tagInfo;
}


class Mp3Tag {
private:
	//-----成员变量-----
	AudioFile* pAudioFile; // 指向音频文件的指针

public:
	//-----成员函数（公有）-----
	explicit Mp3Tag(AudioFile* audioFile);	   // 构造函数
	explicit Mp3Tag(const Mp3Tag & mp3Tag);	   // 复制构造函数
	Mp3Tag& operator =(const Mp3Tag & mp3Tag); // 重载赋值操作符
	~Mp3Tag();								   // 析构函数
	void EditID3v1Tag();					   // 向Mp3文件写入ID3v1标签
	void EditID3v2Tag();					   // 向Mp3文件写入ID3v2标签

private:
	//-----内在实现-----
	void* operator new(size_t size);											    // 重载new操作符，禁止生成堆对象，用以提高程序效率，减低内存管理压力
	void  operator delete(void* pp);											    // 对称的，重载delete操作符，禁止生成堆对象
	void  GenerateId3v1Tag(ID3v1 & id3v1);											// 生成ID3v1标签结构
	void  GenerateId3v2Tag(ID3v2 & id3v2);											// 生成ID3v2标签结构
	void  GenerateId3v2TagHeader(ID3v2TagHeader & id3v2TagHeader, UINT & nTagSize);	// 生成ID3v2标签头结构
	UINT  GenerateId3v2Frame(const UINT & nFrameType, ID3v2Frame & id3v2Frame, 
							 const wchar_t* pContent);							    // 生成ID3v2标签帧结构，返回帧总长度，单位字节
	UINT  GenerateId3v2CommentFrame(ID3v2CommentFrame & id3v2Comment);				// 生成ID3v2注释帧结构，返回帧总长度，单位字节
	UINT  GenerateId3v2PictureFrame(ID3v2PictureFrame & id3v2Picture);				// 生成ID3v2图片帧结构，返回帧总长度，单位字节
	UINT  GenerateId3v2FrameBody(ID3v2FrameBody & id3v2FrameBody, 
								 const wchar_t* pContent);							// 生成ID3v2标签帧内容结构，返回帧内容所占字节数
	UINT  GenerateId3v2FrameHeader(const UINT & nFrameType, 
								   ID3v2FrameHeader & id3v2FrameHeader, 
								   const UINT & nBodyLengthInBytes);				// 生成ID3v2标签帧帧头结构，返回帧总长度，单位字节
};
//-----函数实现（内联）-----
inline		   Mp3Tag::Mp3Tag(AudioFile* audioFile) 
	: pAudioFile(audioFile){
	return;
}
inline         Mp3Tag::Mp3Tag(const Mp3Tag & mp3Tag) 
	: pAudioFile(mp3Tag.pAudioFile){
	return;
}
inline Mp3Tag& Mp3Tag::operator =(const Mp3Tag & mp3Tag) {
	this->pAudioFile = mp3Tag.pAudioFile;
	return *this;
}
inline         Mp3Tag::~Mp3Tag() {
	return;
}
inline void    Mp3Tag::operator delete(void* pp) {
	free(pp);
}
inline void*   Mp3Tag::operator new(size_t size){
	return malloc(size);
}

class ApeTag {
private:
	//-----成员变量-----
	AudioFile* pAudioFile; // 指向音频文件的指针

public:
	//-----成员函数（公有）-----
	explicit ApeTag(AudioFile* audioFile);	   // 构造函数
	explicit ApeTag(const ApeTag & apeTag);	   // 复制构造函数
	ApeTag& operator =(const ApeTag & apeTag); // 重载赋值操作符
	~ApeTag();								   // 析构函数
	void EditAPEv2Tag();					   // 向APE文件写入APEv2标签

private:
	//-----内在实现-----
	void* operator new(size_t size);							           // 重载new操作符，禁止生成堆对象，用以提高程序效率，减低内存管理压力
	void  operator delete(void* pp);							           // 对称的，重载delete操作符，禁止生成堆对象
	void  GenerateAPEv2Tag(APEv2 & apev2);						           // 生成APEv2标签结构
	UINT  GenerateAPEv2Item(const UINT & nItemKey, APEv2Item & apev2Item
							, const wchar_t* pContent);					   // 生成APEv2Item结构，返回该Item总大小，单位：字节
	void  GenerateAPEv2TagHeaderAndFooter(APEv2 & apev2,
										  const UINT & nTagSize,
										  const UINT & nItemCount);        // 生成APEv2标签头/尾结构
};
//-----函数实现（内联）-----
inline		   ApeTag::ApeTag(AudioFile* audioFile) 
	: pAudioFile(audioFile) {
	return;
}
inline         ApeTag::ApeTag(const ApeTag & apeTag) 
	: pAudioFile(apeTag.pAudioFile){
	return;
}
inline ApeTag& ApeTag::operator =(const ApeTag & apeTag) {
	this->pAudioFile = apeTag.pAudioFile;
	return *this;
}
inline         ApeTag::~ApeTag() {
	return;
}
inline void    ApeTag::operator delete(void* pp) {
	free(pp);
}
inline void*   ApeTag::operator new(size_t size){
	return malloc(size);
}

class FlacTag {
private:
	//-----成员变量-----
	AudioFile* pAudioFile; // 指向音频文件的指针

public:
	//-----成员函数（公有）-----
	explicit FlacTag(AudioFile* audioFile);		  // 构造函数
	explicit FlacTag(const FlacTag & flacTag);	  // 复制构造函数
	FlacTag& operator =(const FlacTag & flacTag); // 重载赋值操作符
	~FlacTag();								      // 析构函数
	void EditFLACTag();							  // 向FLAC文件写入FLAC标签

private:
	//-----内在实现-----
	void* operator new(size_t size);							         // 重载new操作符，禁止生成堆对象，用以提高程序效率，减低内存管理压力
	void  operator delete(void* pp);							         // 对称的，重载delete操作符，禁止生成堆对象
	void  GenerateFLACVorbisComment(FLACVorbisComment & vorbisComment);	 // 生成FLAC标签块
	void  GenerateFLACPicture(FLACPicture & picture);					 // 生成FLAC图片块
	UINT  GenerateFLACFrame(const UINT & nFrameType, FLACFrame & frame, 
							const wchar_t* pContent);					 // 生成FLAC标签帧，返回帧长度
};
//-----函数实现（内联）-----
inline		    FlacTag::FlacTag(AudioFile* audioFile)
	: pAudioFile(audioFile) {
	return;
}
inline          FlacTag::FlacTag(const FlacTag & flacTag) 
	: pAudioFile(flacTag.pAudioFile){
	return;
}
inline FlacTag& FlacTag::operator =(const FlacTag & flacTag) {
	this->pAudioFile = flacTag.pAudioFile;
	return *this;
}
inline          FlacTag::~FlacTag() {
	return;
}
inline void     FlacTag::operator delete(void* pp) {
	free(pp);
}
inline void*    FlacTag::operator new(size_t size){
	return malloc(size);
}
