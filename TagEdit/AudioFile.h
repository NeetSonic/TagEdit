//----------------
// ��Ƶ�ļ��� ͷ�ļ�
// 2015.1.4
// Fairchild_Lhy
//----------------
#pragma once 
#include "stdafx.h"
#include <assert.h>

// ��MFC������Ҫ������ͷ�ļ�
// #include <string.h>

// �ο����ף�
// 1��ID3v2��ǩ    �������ĵ� http://id3.org/d3v2.3.0
// 2��APEv2��ǩ    �������ĵ� http://wiki.hydrogenaud.io/index.php?title=APEv2_specification
// 3��FLAC��ʽ	  �������ĵ� https://xiph.org/flac/format.html#def_STREAMINFO
// 4��FLAC��ǩ     �������ĵ� http://www.xiph.org/vorbis/doc/v-comment.html

// �ͷ�ָ���ģ��
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

// ����ԭ������
void DropSpace(CString & cstr);
void UnicodeToAnsi(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert);
void UnicodeToUTF8(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert);
void UintTo4BytesUCharBigEndian(const UINT & uint, UCHAR uchar[4]);
void UintTo3BytesUCharBigEndian(const UINT & uint, UCHAR uchar[3]);

//----------AudioFile����صĶ���----------
// �ļ�����
enum FileType {
	fileType_unknown, // δ֪�ļ�
	fileType_mp3,     // MP3�ļ�
	fileType_ape,     // APE�ļ�
	fileType_flac,    // FLAC�ļ�
	fileType_wav,     // WAV�ļ�
	fileType_png,     // PNG�ļ�
	fileType_jpg_jpeg // JPG,JPEG�ļ�
};

#define TASK_TYPE_COUNT 5 // ��Ƶ�������࣬�����enum FileTypeһ��

// �ļ�������Ŀ
#define _FILE_TYPE_COUNT 7

// �ļ���׺����󳤶ȣ���λ���֣�
#define _MAX_FILE_TYPE_LENGTH 6

// ��Ƶ�ļ���ǩ��Ϣ��󳤶ȣ���λ���֣�
#define _MAX_TITLE_LENGTH  50
#define _MAX_ARTIST_LENGTH 50
#define _MAX_ALBUM_LENGTH  50
#define _MAX_STYLE_LENGTH  50
#define _MAX_YEAR_LENGTH   10

// �ļ���Ϣ�ṹ
struct FileInfo {
	wchar_t chFilePath[_MAX_PATH];			   // �ļ�·��
	wchar_t chFileName[_MAX_PATH];			   // �ļ���
	wchar_t chFileType[_MAX_FILE_TYPE_LENGTH]; // �ļ���׺��
	UINT    nFileType;						   // ��׺����Ӧ���
};

// ��Ƶ�ļ���ǩ��Ϣ�ṹ
struct TagInfo {
	wchar_t chTitle[_MAX_TITLE_LENGTH];   // ����
	wchar_t chArtist[_MAX_ARTIST_LENGTH]; // ������
	wchar_t chAlbum[_MAX_ALBUM_LENGTH];   // ר��
	wchar_t chStyle[_MAX_STYLE_LENGTH];   // ����
	wchar_t chYear[_MAX_YEAR_LENGTH];	  // ���
};

//----------Mp3Tag����صĶ���----------
// ID3v2��ǩ֡����
enum ID3v2FrameType {
	id3v2FrameType_title,   // ����֡
	id3v2FrameType_artist,  // ������֡
	id3v2FrameType_album,   // ר��֡
	id3v2FrameType_year,    // ���֡
	id3v2FrameType_style,   // ����֡
	id3v2FrameType_comment, // ע��֡
	id3v2FrameType_picture  // ͼƬ֡
};

// ID3v2֡��������
#define _ID3V2_FRAME_TYPE_COUNT 7

// ID3v1��ǩ������Ϣ���ȼ��ܳ��ȣ���λ���ֽڣ�
#define _ID3V1_IDENTIFIER_LENGTH 3
#define _ID3V1_YEAR_LENGTH       4
#define _ID3V1_TITLE_LENGTH		 30
#define _ID3V1_ARTIST_LENGTH	 30
#define _ID3V1_ALBUM_LENGTH		 30
#define _ID3V1_COMMENT_LENGTH	 28
#define _ID3V1_LENGTH			 128

// ID3v1��ǩ��MP3�ļ�β����ƫ��������λ���ֽڣ�
#define _ID3V1_OFFSET_FROM_END -_ID3V1_LENGTH

// ID3v2��ǩͷ������Ϣ���ȼ��ܳ��ȣ���λ���ֽڣ�
#define _ID3V2_TAG_HEADER_IDENTIFIER_LENGTH	3
#define _ID3V2_TAG_HEADER_VERSION_LENGTH    2
#define _ID3V2_TAG_HEADER_TAG_SIZE_LENGTH   4
#define _ID3V2_TAG_HEADER_LENGTH			10

// ID3v2֡ͷ������Ϣ���ȼ��ܳ��ȣ���λ���ֽڣ�
#define _ID3V2_FRAME_HEADER_ID_LENGTH         4
#define _ID3V2_FRAME_HEADER_FRAME_SIZE_LENGTH 4
#define _ID3V2_FRAME_HEADER_FLAGS_LENGTH      2
#define _ID3V2_FRAME_HEADER_LENGTH			  10

// ID3v2֡���ݸ�����Ϣ���ȣ���λ���ֽڣ�
#define _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH 2
#define _ID3V2_FRAME_BODY_BODY_LENGTH		    50

// ID3v2ע��֡������Ϣ���ȣ���λ���ֽڣ�
#define _ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH 3  
#define _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH  4 

// ID3v2ͼƬ֡�ṹ������Ϣ���ȣ���λ���ֽڣ�
#define _ID3V2_PICTURE_FRAME_DESCRIP_LENGTH _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH

// ID3v1��ǩ��ʶ��
extern const char _ID3V1_IDENTIFIER[];

// ID3v2��ǩͷ��ʶ��
extern const char _ID3V2_IDENTIFIER[];

// ID3v1��ǩĬ��ע����Ϣ
extern const char* _ID3V1_DEFAULT_COMMENT;

// ID3v2��ǩĬ��ע����Ϣ
extern const wchar_t* _ID3V2_DEFAULT_COMMENT;

// ID3v1��ǩ��ʹ��Genre��Ϣ
#define _ID3V1_GENRE_NOT_USE 0xFF

// ID3v2��ǩ�汾��
extern const char _ID3V2_VERSION[];

// ID3v2֡����ʹ��Unicode������дʱ��Ҫ��ӵ����ڱ�ʶ��ǰ׺
#define _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE 0x01

// ID3v2֡����ʹ��Unicode����д��֮ǰ��Ҫ��ӵ�ǰ׺
extern const UCHAR _ID3V2_FRAME_USE_UNICODE_PREFIX[];

// ID3v2֡ͷ��ʶID���࣬���Ҫ��enum ID3v2FrameType��Ӧ
extern const char _ID3v2_FRAME_HEADER_ID_TYPES[][_ID3V2_FRAME_HEADER_ID_LENGTH];

// ID3v2ע��֡����ʹ�õ�����
extern const char _ID3V2_COMMENT_FRAME_LANGUAGE_CHINESE[];

// ID3v2ע��֡Ĭ������
extern const UCHAR _ID3V2_DEFAULT_COMMENT_DESCRIP[];

// ID3v2ͼƬ֡Ĭ������
#define _ID3V2_DEFAULT_PICTURE_DESCRIP _ID3V2_DEFAULT_COMMENT_DESCRIP

// ID3v2ͼƬ֡ʹ�õ�MIME���Ͷ�Ӧ�Ĵ��룬˳���Ҫ��enum fileType���˳��һ�£�ǰ���ÿ��ַ���ռλ
extern const char* _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE[];

// ID3v2ͼƬ֡ʹ�õ�MIME���Ͷ�Ӧ�Ĵ����ַ������ȣ�������β��0x00����λ���ֽڣ���˳���Ҫ��enum fileType���˳��һ�£�ǰ����0ռλ
extern const UINT _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE_LENGTH[];

// ID3v2ͼƬ֡ʹ�õ�ͼƬ���������ֽڣ�����front��
#define _ID3V2_PICTURE_FRAME_PICTURE_TYPE_COVER_FRONT 0x03

// ID3v1��ǩ�ṹ
struct ID3v1 {
	char  chIdentifier[_ID3V1_IDENTIFIER_LENGTH]; // ��ʶ��
	char  chTitle[_ID3V1_TITLE_LENGTH];			  // ����֡
	char  chArtist[_ID3V1_ARTIST_LENGTH];		  // ������֡
	char  chAlbum[_ID3V1_ALBUM_LENGTH];			  // ר��֡
	char  chYear[_ID3V1_YEAR_LENGTH];			  // ���֡
	char  chComment[_ID3V1_COMMENT_LENGTH];		  // ע��֡
	char  chReserved;						      // ����λ
	char  chTrack;							      // ���죬������ר���е���Ŀ��
	UCHAR chGenre;							      // ���ɱ�ţ�����ID3v1���Ѳ����ã����Բ�ʹ�ô���Ϣ����Ŷ��ձ�� http://baike.baidu.com/view/66078.htm
												  // ����ΪUCHAR����ֹ����ֵ��0xFF����ʾ��ʹ�ã�ʱ���ضϾ���
};

// ID3v2��ǩͷ�ṹ
struct ID3v2TagHeader {
	char  chIdentifier[_ID3V2_TAG_HEADER_IDENTIFIER_LENGTH]; // ��ʶ����Ĭ���ǡ�ID3��
	char  chVersion[_ID3V2_TAG_HEADER_VERSION_LENGTH];		 // �汾��
	char  chFlag;											 // ��־λ��һ�㲻ʹ�ã���0����
	UCHAR chTagSize[_ID3V2_TAG_HEADER_TAG_SIZE_LENGTH];		 // ID3v2��ǩ�ܴ�С����������ǩͷ��10�ֽڣ�����ΪUCHAR����ֹ��ֵʱ���ضϾ���
};

// ID3v2��ǩ֡֡ͷ�ṹ
struct ID3v2FrameHeader {
	char  chFrameID[_ID3V2_FRAME_HEADER_ID_LENGTH];			  // ֡���ͱ�ʶ�����ձ�μ� http://id3.org/d3v2.3.0 ��4.2.1��
	UCHAR chFrameSize[_ID3V2_FRAME_HEADER_FRAME_SIZE_LENGTH]; // ֡���ݳ��ȣ�����֡ͷ10�ֽڣ���Ҫ����֡���ͱ�ʶ����Unicodeǰ׺����3�ֽ�,����ΪUCHAR����ֹ��ֵʱ���ضϾ���
	char  chFlags[_ID3V2_FRAME_HEADER_FLAGS_LENGTH];		  // ֡���ݱ�ʶ����һ�㲻ʹ�ã�Ĭ��Ϊ0x00��0x00
};

// ID3v2��ǩ֡���ݽṹ
struct ID3v2FrameBody {
	char    chEncode;												  // ֡���ݱ��뷽ʽ��Ĭ��ʹ��Unicode���ַ����룬���ַ���Ϊ0x01
	UCHAR   chUnicodePrefix[_ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH]; // ֡����ʹ��Unicodeʱ����Ҫ���ַ���ǰ�ӵ�ǰ׺������ΪUCHAR����ֹ��ֵʱ���ضϾ���
	wchar_t chBody[_ID3V2_FRAME_BODY_BODY_LENGTH];					  // ֡����
	UINT    nBodyLengthInBytes;										  // ֡���ݳ��ȣ��������뷽ʽ��Unicodeǰ׺��
};

// ID3v2��ǩ֡�ṹ
struct ID3v2Frame {
	ID3v2FrameHeader header;		 // ֡ͷ
	ID3v2FrameBody   body;			 // ֡����
};

// ID3v2ע��֡�ṹ������ͨ֡��Ƚ����⡣
struct ID3v2CommentFrame {
	ID3v2FrameHeader header;										           // ֡ͷ
	char             chTextEncoding;										   // ���ݱ��뷽ʽ
	char			 chLanguage[_ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH];		   // ����ʹ�õ����� ������chi �ο�ISO-639-2�淶 http://baike.baidu.com/view/3351971.htm
	UCHAR			 chContentDescrip[_ID3V2_COMMENT_FRAME_DESCRIP_LENGTH];	   // ��ע�͵��������ɱ䳤�ȣ������ǲ���Ҫ������ʹ�ÿ�UNICODE�ַ���FF FE 00 00
	UCHAR			 chUnicodePrefix[_ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH]; // ֡����ʹ��Unicodeʱ����Ҫ���ַ���ǰ�ӵ�ǰ׺������ΪUCHAR����ֹ��ֵʱ���ضϾ���
	wchar_t			 chBody[_ID3V2_FRAME_BODY_BODY_LENGTH];					   // ע������	
	UINT			 nBodyLengthInBytes;									   // ע�����ݳ���
};

// ID3v2ͼƬ֡�ṹ������ͨ֡��Ƚ����⡣
struct ID3v2PictureFrame {
	ID3v2FrameHeader header;										 // ֡ͷ
	char             chTextEncoding;								 // ���ݱ��뷽ʽ
	const char*  	 chMIMEType;									 // ͼƬ��ʽ��Ӧ��MIME���룬Ϊ�ַ�������Ҫ���Ͻ�β��0x00
	UINT             nMIMETypeLengthInBytes;						 // ͼƬ��ʽ��Ӧ��MIME����ĳ��ȣ���Ҫ���Ͻ�β�ַ���ռ��1���ֽ�
	char			 chPictureType;									 // ͼƬ��������������ʹ��0x03��ʾCD����ͼ �������Ͳο� http://id3.org/d3v2.3.0 4.15�½�
	UCHAR			 chDescrip[_ID3V2_PICTURE_FRAME_DESCRIP_LENGTH]; // ��ͼƬ�����������ǲ�ʹ����������ʹ�ÿյ�Unicode�ַ��� 0xFF,0xFE,0x00,0x00
	char*			 chPictureData;									 // ͼƬ����
	UINT			 nPicDataLengthInBytes;							 // ͼƬ���ݵ��ܳ���
};

// ID3v2��ǩ�ṹ
struct ID3v2 {
	ID3v2TagHeader    id3v2TagHeader;    // ��ǩͷ 
	ID3v2Frame	      id3v2FrameTitle;   // ����֡
	ID3v2Frame	      id3v2FrameArtist;  // ������֡
	ID3v2Frame	      id3v2FrameAlbum;   // ר��֡
	ID3v2Frame	      id3v2FrameYear;    // ���֡
	ID3v2Frame	      id3v2FrameStyle;   // ����֡
	ID3v2CommentFrame id3v2FrameComment; // ע��֡
	ID3v2PictureFrame id3v2FramePicture; // ͼƬ֡
};

//----------ApeTag����صĶ���----------
// APEv2ItemKey����
enum APEv2ItemKeyType {
	apev2ItemKeyType_title,  // ����
	apev2ItemKeyType_artist, // ������
	apev2ItemKeyType_album,  // ר��
	apev2ItemKeyType_year,	 // ���
	apev2ItemKeyType_style,  // ���� 
	apev2ItemKeyType_comment // ע��
};

// APEv2ItemKey��������
#define _APEV2_ITEM_KEY_TYPE_COUNT 6

// APEv2��ǩͷ/β������Ϣ���ȼ��ܳ��ȣ���λ���ֽڣ�
#define _APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH 8
#define _APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH	   4
#define _APEV2_TAG_HEADER_FOOTER_RESERVED_LENGTH   8
#define _APEV2_TAG_HEADER_FOOTER_LENGTH			   32

// APEv2Item�ṹ����Ϣ���ȣ���λ���ֽ� ��
#define _APEV2_ITEM_FLAGS_LENGTH 4
#define _APEV2_ITEM_BODY_LENGTH  200 // ���ݱ��룺UTF8

// APEv2��ǩ�汾��2.0
extern const UINT _APEV2_TAG_VERSION_2_0;

// APEv2��ǩĬ��ע����Ϣ
#define _APEV2_DEFAULT_COMMENT _ID3V2_DEFAULT_COMMENT

// APEv2��ǩͷ/β��ʶ����ϸ���òο� http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags
extern const UCHAR _APEV2_TAG_FLAGS_HEADER[];
extern const UCHAR _APEV2_TAG_FLAGS_FOOTER[];

// APEv2��ǩ��ʶ��
extern const char _APEV2_TAG_IDENTIFIER[];

// APEv2ItemKey�����Ͷ�Ӧ�ı�ʶ�����Ҫ��enum APEv2ItemKeyType��Ӧ
extern const char* _APEV2_ITEM_KEY[];

// APEv2ItemKey�����Ͷ�Ӧ�ı�ʶ�ĳ��ȣ����ַ���β��0x00�ֽڣ����Ҫ��enum APEv2ItemKeyType��Ӧ
extern const UINT _APEV2_ITEM_KEY_LENGTH[];

// APEv2��ǩͷ/β�ṹ
struct APEv2TagHeaderFooter {
	char  chIdentifier[_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH]; // ��ʶ�� "APETAGEX"
	UINT  nVersion;											        // �汾�� ����ֵ2000����Ver2.0
	UINT  nTagSize;												    // ��ǩ�ܴ�С����λ���ֽڣ�������ǩͷ������ǩβ+����Items
	UINT  nItemCount;											    // Item����
	UCHAR chFlags[_APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH];			// ��ʶ����ǩ�Ǳ�ǩͷ���Ǳ�ǩβ����Ϣ			
	char  chReserved[_APEV2_TAG_HEADER_FOOTER_RESERVED_LENGTH];		// ����λ��ȫ��0
};
typedef APEv2TagHeaderFooter APEv2TagHeader;
typedef APEv2TagHeaderFooter APEv2TagFooter;

// APEv2Item�ṹ
struct APEv2Item {
	UINT        nBodyLengthInBytes;				   // Item���ݵĳ���
	char	    chFlags[_APEV2_ITEM_FLAGS_LENGTH]; // ��ʶ��������ȫ0����ϸ�ο� http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags 
	const char* pKey;							   // Item����
	UINT        nKeyLengthInBytes;				   // Item���ͳ��ȣ����ַ�����β0x00��һ���ֽ�
	char        chBody[_APEV2_ITEM_BODY_LENGTH];   // Item���� UTF8����
};

// APEv2��ǩ�ṹ
struct APEv2 {
	APEv2TagHeader apev2TagHeader;   // ��ǩͷ
	APEv2Item	   apev2ItemTitle;   // ����
	APEv2Item	   apev2ItemArtist;  // ������
	APEv2Item	   apev2ItemAlbum;   // ר��
	APEv2Item	   apev2ItemYear;    // ���
	APEv2Item	   apev2ItemStyle;   // ����
	APEv2Item	   apev2ItemComment; // ע��
	APEv2TagFooter apev2TagFooter;   // ��ǩβ
};

//----------FlacTag����صĶ���----------
// FLAC�ļ�Block���ͣ��ٷ��涨�����¹̶�˳�򣬲��ɸı䡣����7-126δ���壬127�Ƿ���
enum FLACBlockType {
	flacBlockType_streamInfo,    // 0,����������������һЩ��Ϣ��������ʡ������������������ȡ���һ���ǵ�һ��block���ұ����С�֮����Խ�����block����Щblock���Բ���ʶ��ֱ������
	flacBlockType_padding,       // 1,�����飬���ں�����ӣ�һ����Ϊ�ļ������һ���飬������涨Padding�����������Ϊ���һ����
	flacBlockType_application,   // 2,����������Ӧ�������Ϣ����������32λʶ������flacά����֯�ṩ�ģ���Ψһ��
	flacBlockType_seekTable,     // 3,������ٶ�λ�㣬һ������18bytes��ɣ�2k�Ϳ��Ծ�ȷ��1%�Ķ�λ���������������������λ��
	flacBlockType_vorbisComment, // 4,�洢��һϵ�пɶ��ġ���/ֵ���ļ�ֵ�ԣ�ʹ��UTF-8���롣����flacΨһ�ٷ�֧�ֵı�ǩ��
	flacBlockType_cueSheet,      // 5,�洢����cue sheet�еĸ�����Ϣ�����������������죬�ڱ���CDʱʮ������
	flacBlockType_picture		 // 6,�������ͼƬ��ͬʱ��������url���ֱ��ʵ���Ϣ�������в�ֹһ��picture block
};

// FLAC�ļ�Block������Ŀ
#define _FLAC_BLOCK_TYPE_COUNT 7

// FLAC��ǩ֡����
enum FLACFrameType {
	flacFrameType_title,   // ����֡
	flacFrameType_artist,  // ������֡
	flacFrameType_album,   // ר��֡
	flacFrameType_year,    // ���֡
	flacFrameType_style,   // ����֡
	flacFrameType_comment  // ע��֡
};

// FLAC��ǩ֡������Ŀ
#define _FLAC_FRAME_TYPE_COUNT 6

// FLAC�ļ�ͷ����ʶ���ȣ���λ���ֽڣ�
#define _FLAC_IDENTIFIER_LENGTH 4

// FLAC�ļ�Blockͷ�ṹ�ĸ�����Ϣ���ȼ��ܳ��ȣ���λ���ֽڣ�
#define _FLAC_BLOCK_HEADER_SIZE_LENGTH 3
#define _FLAC_BLOCK_HEADER_LENGTH      4

// FLAC�ļ�Ĭ��Paddind���ȣ���λ���ֽڣ��������������8�ı�������ͷ4�ֽڣ�����ʹ��1KB - 4����1020Bytes��
#define _FLAC_DEFAULT_PADDING_LENGTH 1020

// FLAC�ļ���ǩ֡�ṹ������Ϣ���ȣ���λ���ֽڣ�
#define _FLAC_FRAME_SIZE_LENGTH 4

// FLAC�ļ���ǩ��Ĭ���������ȣ���λ���ֽڣ�
#define _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH 29

// FLAC�ļ���ǩ�����֡���ݳ��ȣ���λ���ֽڣ�
#define _FLAC_FRAME_BODY_LENGTH 200

// FLAC�ļ���ǩ�����������ǩ�Ļ������ȣ���λ���ֽڣ�
#define _FLAC_BLOCK_VORBIS_COMMENT_BASE_LENGTH 37

// FLAC�ļ�ͼƬ�������Ϣ���ȣ���λ���ֽڣ�
#define _FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH         4
#define _FLAC_BLOCK_PICTURE_MIME_SIZE_LENGTH        4
#define _FLAC_BLOCK_PICTURE_DESCRIP_SIZE_LENGTH     4
#define _FLAC_BLOCK_PICTURE_PIC_WIDTH_LENGTH        4
#define _FLAC_BLOCK_PICTURE_PIC_HEIGHT_LENGTH       4
#define _FLAC_BLOCK_PICTURE_COLOR_DEPTH_LENGTH      4
#define _FLAC_BLOCK_PICTURE_INDEX_COLOR_USED_LENGTH 4
#define _FLAC_BLOCK_PICTURE_PIC_DATA_SIZE_LENGTH    4

// FLAC�ļ�ͼƬ��������ȣ���λ���ֽڣ�
#define _FLAC_BLOCK_PICTURE_BASE_LENGTH 32

// FLAC�ļ�ͷ����ʶ��ÿ��FLAC�ļ�һ��ʼ�������У�
extern const char* _FLAC_IDENTIFIER;

// FLAC�ļ������һ������ж�����
extern const UCHAR _FLAC_IS_LAST_BLOCK_MASK;

// FLAC�ļ�ȡ�������͵�����
extern const UCHAR _FLAC_GET_BLOCK_TYPE_MASK;

// FLAC�ļ�Padding����Ϊ���һ�����Block�����ֽ�
extern const UCHAR _FLAC_PADDING_AS_LAST_BLOCK;

// FLAC�ļ���ǩ�������ֽ�
extern const UCHAR _FLAC_BLOCK_VORBIS_COMMENT_TYPE;

// FLAC�ļ�ͼƬ�������ֽ�
extern const UCHAR _FLAC_BLOCK_PICTURE_TYPE;

// FLAC�ļ�Ĭ��Paddind��С�������������8�ı�������ͷ4�ֽڣ�����ʹ��1KB - 4��FLAC�涨��˴�ż�00 03 FC
extern const UCHAR _FLAC_DEFAULT_PADDING_SIZE[];

// FLAC�ļ���ǩ��Ĭ������
extern const char* _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP;

// FLAC�ļ���ǩ��ע��֡Ĭ��ע����Ϣ
#define _FLAC_DEFAULT_COMMENT _ID3V2_DEFAULT_COMMENT

// FLAC�ļ�ͼƬ��ʹ�õ�ͼƬ���������ֽڣ�����front��
extern const char _FLAC_BLOCK_PICTURE_PIC_TYPE_COVER_FRONT[];

// FLAC�ļ�ͼƬ��ʹ�õ�MIME���Ͷ�Ӧ�Ĵ��룬˳���Ҫ��enum fileType���˳��һ�£�ǰ���ÿ��ַ���ռλ
#define _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE

// FLAC�ļ�ͼƬ��ʹ�õ�MIME���Ͷ�Ӧ�Ĵ����ַ������ȣ�������β��0x00����λ���ֽڣ���˳���Ҫ��enum fileType���˳��һ�£�ǰ����0ռλ
extern const UINT _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE_LENGTH[];

// FLAC�ļ���ǩ��֡���ͱ�ʶ��˳��Ҫ��enum FLACFrameTypeһ��
extern const char* _FLAC_FRAME_TYPE_IDENTIFIER[];

// FLAC�ļ�Blockͷ�ṹ
struct FLACBlockHeader {
	UCHAR chType;								  // ����Ϣ���ͣ����λָʾ�˿��ǲ��Ǳ�FLAC�ļ������һ����
	UCHAR chSize[_FLAC_BLOCK_HEADER_SIZE_LENGTH]; // �����ݴ�С 
};

// FLAC�ļ���ͨBlock���ݽṹ
struct FLACBlockBody {
	char*			 chBody;			 // ������
	UINT			 nBodyLengthInBytes; // �������ܳ���
};

// FLAC�ļ���ͨBlock�ṹ���������ǲ����ģ���ֻ��¼��С���ɣ�
struct FLACBlock {
	FLACBlockHeader header; // ��ͷ
	FLACBlockBody	body;   // ������
};

// FLAC�ļ���ǩ֡�ṹ
struct FLACFrame {
	UINT nBodyInBytes;					  // ֡���ݳ���
	char chBody[_FLAC_FRAME_BODY_LENGTH]; // ֡����
};

// FLAC�ļ���ǩ��ṹ
struct FLACVorbisComment {
	FLACBlockHeader header;													// ��ͷ
	UINT            nDescripLengthInBytes;									// ��ǩ����������
	char			chDescrip[_FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH]; // ��ǩ��������UTF-8���룬����Ӣ�ģ���Ansi����һ��
	UINT			nFrameCount;											// ֡�ܸ���
	FLACFrame       title;													// ����֡
	FLACFrame       artist;													// ������֡
	FLACFrame       album;													// ר��֡
	FLACFrame       year;													// ���֡
	FLACFrame       style;													// ����֡
	FLACFrame       comment;												// ע��֡
};

// FLAC�ļ�ͼƬ��ṹ
struct FLACPicture {
	FLACBlockHeader header;													   // ��ͷ
	char		chPicType[_FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH];				   // ͼƬ��������������ʹ��0x00 0x00 0x00 0x03��ʾCD����ͼ �������Ͳο� http://id3.org/d3v2.3.0 4.15�½�
	UCHAR		chMIMESize[_FLAC_BLOCK_PICTURE_MIME_SIZE_LENGTH];			   // MIME���ͷ���С�������ʽ�����
	const char* chMIMEType;													   // MIME���ͷ�
	UINT		nMIMETypeLengthInBytes;										   // MIME���ͷ�����
	char		chDescripSize[_FLAC_BLOCK_PICTURE_DESCRIP_SIZE_LENGTH];		   // ����ͼƬ��UTF-8�ַ�����С������Ĭ�ϲ�����������Ϊ0
	char*	    chDescrip;													   // ����ͼƬ���ַ����������ݲ�ʹ��
	char		chPicWidth[_FLAC_BLOCK_PICTURE_PIC_WIDTH_LENGTH];			   // ͼƬ��ȣ���λ���أ������ʽ����ţ��漰����ͼƬ��ʽ�Ķ�ȡ����ʱ���ܣ���0����
	char		chPicHeight[_FLAC_BLOCK_PICTURE_PIC_HEIGHT_LENGTH];			   // ͼƬ�߶ȣ���λ���أ������ʽ����ţ�ͬ��
	char		chColorDepth[_FLAC_BLOCK_PICTURE_COLOR_DEPTH_LENGTH];		   // ͼƬ��ɫ��ȣ��漰����ͼƬ��ʽ�Ķ�ȡ����ʱ���ܣ���0����
	char		chIndexColorUsed[_FLAC_BLOCK_PICTURE_INDEX_COLOR_USED_LENGTH]; // ʹ����ɫ�������������漰����ͼƬ��ʽ�Ķ�ȡ����ʱ���ܣ���0����
	UCHAR	    chPicDataSize[_FLAC_BLOCK_PICTURE_PIC_DATA_SIZE_LENGTH];	   // ͼƬ���ݴ�С�������ʽ�����
	char*	    chPicData;													   // ͼƬ����
	UINT		nPicDataLengthInBytes;										   // ͼƬ���ݳ���
};

// FLAC�ļ�ͷ�ṹ
struct FLAC {
	char			  chIdentifier[_FLAC_IDENTIFIER_LENGTH]; // FLAC�ļ�ͷ��ʶ��
	FLACBlock		  BlockList[_FLAC_BLOCK_TYPE_COUNT];     // Block�б�ֻ������ǲ����ĵ���ͨBlock����Ϣ
	FLACVorbisComment vorbisComment;						 // FLAC��ǩ
	FLACPicture		  picture;								 // ����ͼƬ
};


class AudioFile {
private:
	//-----��Ա����-----
	FileInfo	 audioInfo; // ��Ƶ�ļ���Ϣ
	TagInfo		 tagInfo;   // ��Ƶ�ļ���ǩ��Ϣ
	FileInfo     picInfo;   // ��Ƶ��������ͼ�ļ���Ϣ

public:
	//-----��Ա���������У�-----
	AudioFile();									   // Ĭ�Ϲ��캯��
	~AudioFile();									   // ��������
	explicit AudioFile(const AudioFile & audioFile);   // ���ƹ��캯��
	AudioFile& operator=(const AudioFile & audioFile); // ���ظ�ֵ������
	BOOL Open(const wchar_t* filePath);				   // ����Ƶ�ļ������ļ����ж�ȡ����ǩ��Ϣ
	BOOL AddCoverPicture(const wchar_t* picPath);	   // ��ӷ���ͼƬ
	const FileInfo& GetAudioInfo();					   // ��ȡ��Ƶ�ļ���Ϣ
	const FileInfo& GetPicInfo();					   // ��ȡ����ͼƬ�ļ���Ϣ
	const TagInfo&  GetTagInfo();					   // ��ȡ��Ƶ�ļ���ǩ��Ϣ
	void  StartEdit();								   // ��ʼ�޸ı�ǩ��Ϣ

private:
	//-----����ʵ��-----
	void GenerateFileInfo(const wchar_t* filePath, FileInfo & info); // �����ļ�����·�������ļ���Ϣ�ṹ
	void ReadTagInfoFromFileName();								     // ���ļ�����ȡ��ǩ��Ϣ

private:
	void* operator new(size_t size){return malloc(size);} // ����new����������ֹ���ɶѶ���������߳���Ч�ʣ������ڴ����ѹ��
	void  operator delete(void* pp) { free(pp); }		  // �ԳƵģ�����delete����������ֹ���ɶѶ���
};
//-----����ʵ�֣�������-----
inline		           AudioFile::AudioFile(){
	memset(&tagInfo, 0, sizeof(tagInfo));	  // ��ʼ����Ƶ�ļ���Ϣ
	memset(&audioInfo, 0, sizeof(audioInfo)); // ��ʼ����Ƶ�ļ���ǩ��Ϣ
	memset(&picInfo, 0, sizeof(picInfo));	  // ��ʼ����Ƶ��������ͼ�ļ���Ϣ
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
	// ����Ƿ��Ѿ�ӵ�кϷ�����Ƶ�ļ���Ϣ
	assert(PathFileExists(audioInfo.chFilePath)); // ���û�ж�Ӧ·�����ļ��򱨴���ֹ����
	assert(wcslen(audioInfo.chFileName));         // ����ļ�������Ϊ0�򱨴���ֹ����
	assert(wcslen(audioInfo.chFileName));		  // �����׺������Ϊ0�򱨴���ֹ����
	assert(audioInfo.nFileType != type_unknown);  // �����׺������ʶ���򱨴���ֹ����

	return audioInfo;
}
inline const FileInfo& AudioFile::GetPicInfo() {
	// ����Ƿ��Ѿ�ӵ�кϷ�����Ƶ�ļ���Ϣ
	//assert(PathFileExists(picInfo.chFilePath)); // ���û�ж�Ӧ·�����ļ��򱨴���ֹ����
	//assert(wcslen(picInfo.chFileName));         // ����ļ�������Ϊ0�򱨴���ֹ����
	//assert(wcslen(picInfo.chFileName));		    // �����׺������Ϊ0�򱨴���ֹ����
	//assert(picInfo.nFileType != type_unknown);  // �����׺������ʶ���򱨴���ֹ����

	return picInfo;
}
inline const TagInfo&  AudioFile::GetTagInfo() {
	return tagInfo;
}


class Mp3Tag {
private:
	//-----��Ա����-----
	AudioFile* pAudioFile; // ָ����Ƶ�ļ���ָ��

public:
	//-----��Ա���������У�-----
	explicit Mp3Tag(AudioFile* audioFile);	   // ���캯��
	explicit Mp3Tag(const Mp3Tag & mp3Tag);	   // ���ƹ��캯��
	Mp3Tag& operator =(const Mp3Tag & mp3Tag); // ���ظ�ֵ������
	~Mp3Tag();								   // ��������
	void EditID3v1Tag();					   // ��Mp3�ļ�д��ID3v1��ǩ
	void EditID3v2Tag();					   // ��Mp3�ļ�д��ID3v2��ǩ

private:
	//-----����ʵ��-----
	void* operator new(size_t size);											    // ����new����������ֹ���ɶѶ���������߳���Ч�ʣ������ڴ����ѹ��
	void  operator delete(void* pp);											    // �ԳƵģ�����delete����������ֹ���ɶѶ���
	void  GenerateId3v1Tag(ID3v1 & id3v1);											// ����ID3v1��ǩ�ṹ
	void  GenerateId3v2Tag(ID3v2 & id3v2);											// ����ID3v2��ǩ�ṹ
	void  GenerateId3v2TagHeader(ID3v2TagHeader & id3v2TagHeader, UINT & nTagSize);	// ����ID3v2��ǩͷ�ṹ
	UINT  GenerateId3v2Frame(const UINT & nFrameType, ID3v2Frame & id3v2Frame, 
							 const wchar_t* pContent);							    // ����ID3v2��ǩ֡�ṹ������֡�ܳ��ȣ���λ�ֽ�
	UINT  GenerateId3v2CommentFrame(ID3v2CommentFrame & id3v2Comment);				// ����ID3v2ע��֡�ṹ������֡�ܳ��ȣ���λ�ֽ�
	UINT  GenerateId3v2PictureFrame(ID3v2PictureFrame & id3v2Picture);				// ����ID3v2ͼƬ֡�ṹ������֡�ܳ��ȣ���λ�ֽ�
	UINT  GenerateId3v2FrameBody(ID3v2FrameBody & id3v2FrameBody, 
								 const wchar_t* pContent);							// ����ID3v2��ǩ֡���ݽṹ������֡������ռ�ֽ���
	UINT  GenerateId3v2FrameHeader(const UINT & nFrameType, 
								   ID3v2FrameHeader & id3v2FrameHeader, 
								   const UINT & nBodyLengthInBytes);				// ����ID3v2��ǩ֡֡ͷ�ṹ������֡�ܳ��ȣ���λ�ֽ�
};
//-----����ʵ�֣�������-----
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
	//-----��Ա����-----
	AudioFile* pAudioFile; // ָ����Ƶ�ļ���ָ��

public:
	//-----��Ա���������У�-----
	explicit ApeTag(AudioFile* audioFile);	   // ���캯��
	explicit ApeTag(const ApeTag & apeTag);	   // ���ƹ��캯��
	ApeTag& operator =(const ApeTag & apeTag); // ���ظ�ֵ������
	~ApeTag();								   // ��������
	void EditAPEv2Tag();					   // ��APE�ļ�д��APEv2��ǩ

private:
	//-----����ʵ��-----
	void* operator new(size_t size);							           // ����new����������ֹ���ɶѶ���������߳���Ч�ʣ������ڴ����ѹ��
	void  operator delete(void* pp);							           // �ԳƵģ�����delete����������ֹ���ɶѶ���
	void  GenerateAPEv2Tag(APEv2 & apev2);						           // ����APEv2��ǩ�ṹ
	UINT  GenerateAPEv2Item(const UINT & nItemKey, APEv2Item & apev2Item
							, const wchar_t* pContent);					   // ����APEv2Item�ṹ�����ظ�Item�ܴ�С����λ���ֽ�
	void  GenerateAPEv2TagHeaderAndFooter(APEv2 & apev2,
										  const UINT & nTagSize,
										  const UINT & nItemCount);        // ����APEv2��ǩͷ/β�ṹ
};
//-----����ʵ�֣�������-----
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
	//-----��Ա����-----
	AudioFile* pAudioFile; // ָ����Ƶ�ļ���ָ��

public:
	//-----��Ա���������У�-----
	explicit FlacTag(AudioFile* audioFile);		  // ���캯��
	explicit FlacTag(const FlacTag & flacTag);	  // ���ƹ��캯��
	FlacTag& operator =(const FlacTag & flacTag); // ���ظ�ֵ������
	~FlacTag();								      // ��������
	void EditFLACTag();							  // ��FLAC�ļ�д��FLAC��ǩ

private:
	//-----����ʵ��-----
	void* operator new(size_t size);							         // ����new����������ֹ���ɶѶ���������߳���Ч�ʣ������ڴ����ѹ��
	void  operator delete(void* pp);							         // �ԳƵģ�����delete����������ֹ���ɶѶ���
	void  GenerateFLACVorbisComment(FLACVorbisComment & vorbisComment);	 // ����FLAC��ǩ��
	void  GenerateFLACPicture(FLACPicture & picture);					 // ����FLACͼƬ��
	UINT  GenerateFLACFrame(const UINT & nFrameType, FLACFrame & frame, 
							const wchar_t* pContent);					 // ����FLAC��ǩ֡������֡����
};
//-----����ʵ�֣�������-----
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
