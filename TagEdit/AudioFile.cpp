//-----------------
// ��Ƶ�ļ��� ʵ���ļ�
// 2015.1.4
// Fairchild_Lhy
//-----------------
#include "AudioFile.h"

//----------����ȫ�ֱ���----------
// ID3v1��ǩ��ʶ��
const char _ID3V1_IDENTIFIER[_ID3V1_IDENTIFIER_LENGTH] = {'T', 'A', 'G'};

// ID3v2��ǩͷ��ʶ��
const char _ID3V2_IDENTIFIER[_ID3V2_TAG_HEADER_IDENTIFIER_LENGTH] = {'I', 'D', '3'};

// ID3v1��ǩĬ��ע����Ϣ
const char* _ID3V1_DEFAULT_COMMENT = "����ǩ��TagEdit v1.0����";

// ID3v2��ǩĬ��ע����Ϣ
const wchar_t* _ID3V2_DEFAULT_COMMENT = _T("����ǩ��TagEdit v1.0����");

// ID3v2��ǩ�汾��
const char _ID3V2_VERSION[_ID3V2_TAG_HEADER_VERSION_LENGTH] = {0x03, 0x00}; // ����ID3v2.3.0�汾

// ID3v2֡����ʹ��Unicode����д��֮ǰ��Ҫ��ӵ�ǰ׺
const UCHAR _ID3V2_FRAME_USE_UNICODE_PREFIX[_ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH] = {0xFF, 0xFE};

// ID3v2֡ͷ��ʶID���࣬���Ҫ��enum ID3v2FrameType��Ӧ
const char _ID3v2_FRAME_HEADER_ID_TYPES[_ID3V2_FRAME_TYPE_COUNT][_ID3V2_FRAME_HEADER_ID_LENGTH] = {
		{'T', 'I', 'T', '2'}, {'T', 'P', 'E', '1'}, {'T', 'A', 'L', 'B'}, {'T', 'Y', 'E', 'R'},
		{'T', 'C', 'O', 'N'}, {'C', 'O', 'M', 'M'}, {'A', 'P', 'I', 'C'}
};

// ID3v2ע��֡����ʹ�õ�����
const char _ID3V2_COMMENT_FRAME_LANGUAGE_CHINESE[_ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH] = {'c', 'h', 'i'};

// ID3v2ע��֡Ĭ������
const UCHAR _ID3V2_DEFAULT_COMMENT_DESCRIP[_ID3V2_COMMENT_FRAME_DESCRIP_LENGTH] = {0xFF, 0xFE, 0x00, 0x00};

// ID3v2ͼƬ֡ʹ�õ�MIME���Ͷ�Ӧ�Ĵ��룬˳���Ҫ��enum fileType���˳��һ�£�ǰ���ÿ��ַ���ռλ
const char* _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE[_FILE_TYPE_COUNT] = {
	NULL, NULL, NULL, NULL, NULL, "image/png", "image/jpeg"
};

// ID3v2ͼƬ֡ʹ�õ�MIME���Ͷ�Ӧ�Ĵ����ַ������ȣ�������β��0x00����λ���ֽڣ���˳���Ҫ��enum fileType���˳��һ�£�ǰ����0ռλ
const UINT _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE_LENGTH[_FILE_TYPE_COUNT] = {
	0, 0, 0, 0, 0, 10, 11
};

// APEv2��ǩ�汾��2.0
const UINT _APEV2_TAG_VERSION_2_0 = 2000;

// APEv2��ǩͷ/β��ʶ����ϸ���òο� http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags
const UCHAR _APEV2_TAG_FLAGS_HEADER[_APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH] = {0x00, 0x00, 0x00, 0xA0};
const UCHAR _APEV2_TAG_FLAGS_FOOTER[_APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH] = {0x00, 0x00, 0x00, 0x80};

// APEv2��ǩ��ʶ��
const char _APEV2_TAG_IDENTIFIER[_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH] = {
	'A', 'P', 'E', 'T', 'A', 'G', 'E', 'X'
};

// APEv2ItemKey�����Ͷ�Ӧ�ı�ʶ�����Ҫ��enum APEv2ItemKeyType��Ӧ
const char* _APEV2_ITEM_KEY[_APEV2_ITEM_KEY_TYPE_COUNT] = {
	"Title", "Artist", "Album", "Year", "Genre", "Comment"
};

// APEv2ItemKey�����Ͷ�Ӧ�ı�ʶ�ĳ��ȣ����ַ���β��0x00�ֽڣ����Ҫ��enum APEv2ItemKeyType��Ӧ
const UINT _APEV2_ITEM_KEY_LENGTH[_APEV2_ITEM_KEY_TYPE_COUNT] = {
	6, 7, 6, 5, 6, 8
};

// FLAC�ļ�ͷ����ʶ��ÿ��FLAC�ļ�һ��ʼ�������У�
const char* _FLAC_IDENTIFIER = "fLaC";

// FLAC�ļ������һ������ж�����
const UCHAR _FLAC_IS_LAST_BLOCK_MASK = 0x80;

// FLAC�ļ�ȡ�������͵�����
const UCHAR _FLAC_GET_BLOCK_TYPE_MASK = 0x7F;

// FLAC�ļ�Padding����Ϊ���һ�����Block�����ֽ�
const UCHAR _FLAC_PADDING_AS_LAST_BLOCK = 0x81;

// FLAC�ļ���ǩ�������ֽ�
const UCHAR _FLAC_BLOCK_VORBIS_COMMENT_TYPE = 0x04;

// FLAC�ļ�ͼƬ�������ֽ�
const UCHAR _FLAC_BLOCK_PICTURE_TYPE = 0x06;

// FLAC�ļ�Ĭ��Paddind��С�������������8�ı�������ͷ4�ֽڣ�����ʹ��1KB - 4��FLAC�涨��˴�ż�00 03 FC
const UCHAR _FLAC_DEFAULT_PADDING_SIZE[_FLAC_BLOCK_HEADER_SIZE_LENGTH] = {0x00, 0x03, 0xFC};

// FLAC�ļ���ǩ��Ĭ������
const char* _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP = "Xiph.Org libVorbis I 20020717";

// FLAC�ļ�ͼƬ��ʹ�õ�ͼƬ���������ֽڣ�����front��
const char _FLAC_BLOCK_PICTURE_PIC_TYPE_COVER_FRONT[_FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH] = {0x00, 0x00, 0x00, 0x03};

// FLAC�ļ�ͼƬ��ʹ�õ�MIME���Ͷ�Ӧ�Ĵ����ַ������ȣ�������β��0x00����λ���ֽڣ���˳���Ҫ��enum fileType���˳��һ�£�ǰ����0ռλ
const UINT _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE_LENGTH[_FILE_TYPE_COUNT] = {
	0, 0, 0, 0, 0, 9, 10
};

// FLAC�ļ���ǩ��֡���ͱ�ʶ��˳��Ҫ��enum FLACFrameTypeһ��
const char* _FLAC_FRAME_TYPE_IDENTIFIER[_FLAC_FRAME_TYPE_COUNT] {
	"TITLE=", "ARTIST=", "ALBUM=", "DATE=", "GENRE=", "DESCRIPTION="
};

// ��MP3�ļ�д��ID3v2��ͨ֡�ķ�ʽ
#define WriteID3v2Frame(file, frame)\
	if(frame.body.nBodyLengthInBytes){\
		file.Write(&frame.header, _ID3V2_FRAME_HEADER_LENGTH);\
		file.Write(&frame.body.chEncode, sizeof(frame.body.chEncode));\
		file.Write(frame.body.chUnicodePrefix, _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);\
		file.Write(frame.body.chBody, frame.body.nBodyLengthInBytes);\
	}

// ��MP3�ļ�д��ID3v2ע��֡�ķ�ʽ
#define WriteID3v2CommentFrame(file, frame)\
	file.Write(&frame.header, _ID3V2_FRAME_HEADER_LENGTH);\
	file.Write(&frame.chTextEncoding, sizeof(frame.chTextEncoding));\
	file.Write(frame.chLanguage, _ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH);\
	file.Write(frame.chContentDescrip, _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH);\
	file.Write(frame.chUnicodePrefix, _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);\
	file.Write(frame.chBody, frame.nBodyLengthInBytes)

// ��MP3�ļ�д��ID3v2ͼƬ֡�ķ�ʽ
#define WriteID3v2PictureFrame(file, frame)\
	if(frame.nPicDataLengthInBytes){\
		file.Write(&frame.header, _ID3V2_FRAME_HEADER_LENGTH);\
		file.Write(&frame.chTextEncoding, sizeof(frame.chTextEncoding));\
		file.Write(frame.chMIMEType, frame.nMIMETypeLengthInBytes);\
		file.Write(&frame.chPictureType, sizeof(frame.chPictureType));\
		file.Write(frame.chDescrip, _ID3V2_PICTURE_FRAME_DESCRIP_LENGTH);\
		file.Write(frame.chPictureData, frame.nPicDataLengthInBytes);\
		MultiDelete(frame.chPictureData);\
	}

// ��MP3�ļ�д��ID3v2��ǩ�ķ�ʽ
#define WriteID3v2(file, id3v2)\
	file.Write(&id3v2.id3v2TagHeader, _ID3V2_TAG_HEADER_LENGTH);\
	WriteID3v2Frame(file, id3v2.id3v2FrameTitle);\
	WriteID3v2Frame(file, id3v2.id3v2FrameArtist);\
	WriteID3v2Frame(file, id3v2.id3v2FrameAlbum);\
	WriteID3v2Frame(file, id3v2.id3v2FrameYear);\
	WriteID3v2Frame(file, id3v2.id3v2FrameStyle);\
	WriteID3v2CommentFrame(file, id3v2.id3v2FrameComment);\
	WriteID3v2PictureFrame(file, id3v2.id3v2FramePicture)

// ��APE�ļ�д��APEv2Item�ķ�ʽ
#define WriteAPEv2Item(file, item)\
	if(item.nBodyLengthInBytes){\
		file.Write(&item.nBodyLengthInBytes, sizeof(item.nBodyLengthInBytes));\
		file.Write(item.chFlags, _APEV2_ITEM_FLAGS_LENGTH);\
		file.Write(item.pKey, item.nKeyLengthInBytes);\
		file.Write(item.chBody, item.nBodyLengthInBytes);\
	}

// ��APE�ļ�д��APEv2��ǩ�ķ�ʽ
#define WriteAPEv2(file, apev2)\
	file.Write(&apev2.apev2TagHeader, _APEV2_TAG_HEADER_FOOTER_LENGTH);\
	WriteAPEv2Item(file, apev2.apev2ItemTitle);\
	WriteAPEv2Item(file, apev2.apev2ItemArtist);\
	WriteAPEv2Item(file, apev2.apev2ItemAlbum);\
	WriteAPEv2Item(file, apev2.apev2ItemYear);\
	WriteAPEv2Item(file, apev2.apev2ItemStyle);\
	WriteAPEv2Item(file, apev2.apev2ItemComment);\
	file.Write(&apev2.apev2TagFooter, _APEV2_TAG_HEADER_FOOTER_LENGTH)

// ��FLAC�ļ�д��һ���ķ�ʽ
#define WriteFLACBlock(file, block)\
	if(block.body.nBodyLengthInBytes){\
		file.Write(&block.header, _FLAC_BLOCK_HEADER_LENGTH);\
		file.Write(block.body.chBody, block.body.nBodyLengthInBytes);\
		MultiDelete(block.body.chBody);\
	}

// ��FLAC�ļ�д���ǩ֡�ķ�ʽ
#define WriteFLACFrame(file, frame)\
	if(frame.nBodyInBytes){\
		file.Write(&frame.nBodyInBytes, sizeof(frame.nBodyInBytes));\
		file.Write(frame.chBody, frame.nBodyInBytes);\
	}

// ��FLAC�ļ�д���ǩ��ķ�ʽ
#define WriteFLACVorbisComment(file, vorbisComment)\
	file.Write(&vorbisComment.header, _FLAC_BLOCK_HEADER_LENGTH);\
	file.Write(&vorbisComment.nDescripLengthInBytes, sizeof(vorbisComment.nDescripLengthInBytes));\
	file.Write(vorbisComment.chDescrip, _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH);\
	file.Write(&vorbisComment.nFrameCount, sizeof(vorbisComment.nFrameCount));\
	WriteFLACFrame(file, vorbisComment.title);\
	WriteFLACFrame(file, vorbisComment.artist);\
	WriteFLACFrame(file, vorbisComment.album);\
	WriteFLACFrame(file, vorbisComment.year);\
	WriteFLACFrame(file, vorbisComment.style);\
	WriteFLACFrame(file, vorbisComment.comment)

// ��FLAC�ļ�д��ͼƬ��ķ�ʽ
#define WriteFLACPicture(file, picture)\
	if(picture.nPicDataLengthInBytes){\
		file.Write(&picture.header, _FLAC_BLOCK_HEADER_LENGTH);\
		file.Write(picture.chPicType, _FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH);\
		file.Write(picture.chMIMESize, _FLAC_BLOCK_PICTURE_MIME_SIZE_LENGTH);\
		file.Write(picture.chMIMEType, picture.nMIMETypeLengthInBytes);\
		file.Write(picture.chDescripSize, _FLAC_BLOCK_PICTURE_DESCRIP_SIZE_LENGTH);\
		file.Write(picture.chPicWidth, _FLAC_BLOCK_PICTURE_PIC_WIDTH_LENGTH);\
		file.Write(picture.chPicHeight, _FLAC_BLOCK_PICTURE_PIC_HEIGHT_LENGTH);\
		file.Write(picture.chColorDepth, _FLAC_BLOCK_PICTURE_COLOR_DEPTH_LENGTH);\
		file.Write(picture.chIndexColorUsed, _FLAC_BLOCK_PICTURE_INDEX_COLOR_USED_LENGTH);\
		file.Write(picture.chPicDataSize, _FLAC_BLOCK_PICTURE_PIC_DATA_SIZE_LENGTH);\
		file.Write(picture.chPicData, picture.nPicDataLengthInBytes);\
		MultiDelete(picture.chPicData);\
	}

// ��FLAC�ļ�д��FLAC�ļ�ͷ�ķ�ʽ
#define WriteFLAC(file, flac)\
	file.Write(flac.chIdentifier, _FLAC_IDENTIFIER_LENGTH);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_streamInfo]);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_application]);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_seekTable]);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_cueSheet]);\
	WriteFLACVorbisComment(file, flac.vorbisComment);\
	WriteFLACPicture(file, flac.picture);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_padding])

// ͨ�ù��ܺ���
inline void UnicodeToAnsi(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert) {
	int nLen = WideCharToMultiByte(CP_ACP, 0, srcStr, -1, NULL, 0, NULL, NULL); // �õ�ת�����Ansi�ַ������ȣ�������β��\0
	if(nLen){
		char* p = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, srcStr, -1, p, nLen, NULL, NULL); // ת��
		strncpy(destStr, p, nMaxBytesToConvert); // ��nMaxBytesToConvert��Χ������\0����涼���\0
		MultiDelete(p);
	}
}
inline void UnicodeToUTF8(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert) {
	int nLen = WideCharToMultiByte(CP_UTF8, 0, srcStr, -1, NULL, 0, NULL, NULL); 
	if(nLen){
		char* utf8Str = new char[nLen];
		WideCharToMultiByte(CP_UTF8, 0, srcStr, -1, utf8Str, nLen, NULL, NULL); 
		strncpy(destStr, utf8Str, nMaxBytesToConvert);
		MultiDelete(utf8Str);
	}
}
inline void UintTo4BytesUCharBigEndian(const UINT & uint, UCHAR uchar[4]) {
	uchar[0] = static_cast<UCHAR>(uint >> 24);
	uchar[1] = static_cast<UCHAR>((uint & 0x00FF0000) >> 16);
	uchar[2] = static_cast<UCHAR>((uint & 0x0000FF00) >> 8);
	uchar[3] = static_cast<UCHAR>(uint & 0x000000FF);
}
inline void UintTo3BytesUCharBigEndian(const UINT & uint, UCHAR uchar[3]) {
	uchar[0] = static_cast<UCHAR>((uint & 0x00FF0000) >> 16);
	uchar[1] = static_cast<UCHAR>((uint & 0x0000FF00) >> 8);
	uchar[2] = static_cast<UCHAR>(uint & 0x000000FF);
}
	   void DropSpace(CString & cstr) {
	// ��һ��ѭ��ȷ��cstr���������Ϊ0����������λ�����ַ�' '
	while(cstr.GetAt(0) == ' '){ cstr = cstr.Right(cstr.GetLength() - 1); }
	while((cstr.GetLength() > 1) && (cstr.GetAt(cstr.GetLength() - 1) == ' ')){ cstr = cstr.Left(cstr.GetLength() - 1); }
}

// ��������
void Mp3Task(AudioFile* audioFile) {
	Mp3Tag task(audioFile);
	task.EditID3v1Tag();
	task.EditID3v2Tag();
}
void ApeTask(AudioFile* audioFile) {
	ApeTag task(audioFile);
	task.EditAPEv2Tag();
}
void FlacTask(AudioFile* audioFile) {
	FlacTag task(audioFile);
	task.EditFLACTag();
}
void WavTask(AudioFile* audioFile) {
}
void(*AudioTask[TASK_TYPE_COUNT])(AudioFile* audioFile) = {NULL, Mp3Task, ApeTask, FlacTask, WavTask};

// �ຯ������
BOOL AudioFile::Open(const wchar_t* filePath) {
	if(!PathFileExists(filePath)) return FALSE; // �ж��ļ��Ƿ���ڣ������ڼ�����FALSE��ʾʧ��
	GenerateFileInfo(filePath, audioInfo);      // ����FileInfo�ṹ
	ReadTagInfoFromFileName();				    // ���ļ����л�ȡ��ǩ��Ϣ������TagInfo�ṹ
	return TRUE;
}
BOOL AudioFile::AddCoverPicture(const wchar_t* picPath) {
	if(!PathFileExists(picPath)) return FALSE; // �ж��ļ��Ƿ���ڣ������ڼ�����FALSE��ʾʧ��
	GenerateFileInfo(picPath, picInfo);        // ����FileInfo�ṹ
	return TRUE;
}
void AudioFile::ReadTagInfoFromFileName() {
	CString csFileName(audioInfo.chFileName);
	CString csTitle, csArtist, csAlbum, csYear, csStyle;
	int		nCurrFlag;						  // ��ǰ�ָ�����λ��
	int		nLastFlag = 0;					  // ��һ�ηָ�����λ��
	int		nCount;						      // ��Ž�ȡ�ַ��ĸ���
	int		nLength = csFileName.GetLength(); // �ļ����ܳ���

	//----------ȷ������----------
	nCurrFlag = csFileName.Find('-', nLastFlag);
	if(nCurrFlag == -1){
		//AddErrCode(NAME_ERR_01); // �ļ���û�зָ���,�������û�и�����Ϣ
		csTitle = csFileName;
		DropSpace(csTitle);
		wcsncpy(tagInfo.chTitle, csTitle, _MAX_TITLE_LENGTH); // ��wcsncpy����ΪlstrcpynW�����������涨���ֵ�β���Զ���Ϊ0x0000
		return;
	}
	else{
		nCount = nCurrFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_02); } // �ļ����ָ�����������λ,�������û�б�����Ϣ
		csTitle = csFileName.Left(nCount);
		DropSpace(csTitle);
		wcsncpy(tagInfo.chTitle, csTitle, _MAX_TITLE_LENGTH);
	}

	//----------ȷ��������----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	if(nCurrFlag == -1){ // ԭ�ļ�����ʽΪ"������-������"
		nCount = nLength - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_03); } // �ļ����Էָ�����β�������û�и�����Ϣ
		csArtist = csFileName.Right(nCount);
		DropSpace(csArtist);
		wcsncpy(tagInfo.chArtist, csArtist, _MAX_ARTIST_LENGTH);
		return;
	}
	else{ // nCurrFlag != -1, ��ʾ���滹��ר����Ϣ
		nCount = nCurrFlag - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_04); } // �ļ�������2���ָ������������,�������û�и�����Ϣ
		csArtist = csFileName.Mid(nLastFlag, nCount);
		DropSpace(csArtist);
		wcsncpy(tagInfo.chArtist, csArtist, _MAX_ARTIST_LENGTH);
	}

	//----------ȷ��ר��----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	if(nCurrFlag == -1){ // ԭ�ļ�����ʽΪ"������-������-ר��"
		nCount = nLength - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_05); } // �ļ����Էָ�����β�������û��ר����Ϣ
		csAlbum = csFileName.Right(nCount);
		DropSpace(csAlbum);
		wcsncpy(tagInfo.chAlbum, csAlbum, _MAX_ALBUM_LENGTH);
		return;
	}
	else{ // nCurrFlag != -1, ��ʾ���滹�������Ϣ
		nCount = nCurrFlag - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_06); } // �ļ�������2���ָ������������,�������û��ר����Ϣ
		csAlbum = csFileName.Mid(nLastFlag, nCount);
		DropSpace(csAlbum);
		wcsncpy(tagInfo.chAlbum, csAlbum, _MAX_ALBUM_LENGTH);
	}

	//----------ȷ�����----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	if(nCurrFlag == -1){ // ԭ�ļ�����ʽΪ"������-������-ר��-���"
		nCount = nLength - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_07); } // �ļ����Էָ�����β�������û�������Ϣ
		csYear = csFileName.Right(nCount);
		DropSpace(csYear);
		wcsncpy(tagInfo.chYear, csYear, _MAX_ALBUM_LENGTH);
		return;
	}
	else{ // nCurrFlag != -1, ��ʾ���滹��������Ϣ
		nCount = nCurrFlag - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_08); } // �ļ�������2���ָ������������,�������û�������Ϣ
		csYear = csFileName.Mid(nLastFlag, nCount);
		DropSpace(csYear);
		wcsncpy(tagInfo.chYear, csYear, _MAX_ALBUM_LENGTH);
	}

	//----------ȷ������----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	//if(nCurrFlag != -1) { AddErrCode(NAME_ERR_09); } // �ļ������ж���ķָ���,����������б�ǩ��Ϣ����ȷ
	nCount = nLength - nLastFlag;
	//if(nCount == 0){ AddErrCode(NAME_ERR_10); } // �ļ����Էָ�����β�������û��������Ϣ
	csStyle = csFileName.Right(nCount);
	DropSpace(csStyle);
	wcsncpy(tagInfo.chStyle, csStyle, _MAX_ALBUM_LENGTH);
}
void AudioFile::GenerateFileInfo(const wchar_t* filePath, FileInfo & info) {
	wcsncpy(info.chFilePath, filePath, _MAX_PATH); // �ڶ�Ӧ�����ڴ����Ч���ļ�·��
	CString cs(filePath);
	CString csName, csType;
	cs = cs.Right(cs.GetLength() - cs.ReverseFind('\\') - 1);
	int nFlag = cs.ReverseFind('.');
	csName = cs.Left(nFlag);
	csType = cs.Right(cs.GetLength() - nFlag - 1);
	csType = csType.MakeLower();
	wcsncpy(info.chFileName, csName, _MAX_PATH);
	wcsncpy(info.chFileType, csType, _MAX_FILE_TYPE_LENGTH);

	// Ϊ��׺����ţ�����ʶ��
	// ��Ƶ�ļ�
	if(!wcsncmp(info.chFileType, _T("mp3"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_mp3; }
	else if(!wcsncmp(info.chFileType, _T("ape"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_ape; }
	else if(!wcsncmp(info.chFileType, _T("flac"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_flac; }
	else if(!wcsncmp(info.chFileType, _T("wav"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_wav; }
	// ͼƬ�ļ�
	else if(!wcsncmp(info.chFileType, _T("png"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_png; }
	else if(!wcsncmp(info.chFileType, _T("jpg"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_jpg_jpeg; }
	else if(!wcsncmp(info.chFileType, _T("jpeg"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_jpg_jpeg; }
}
void AudioFile::StartEdit() {
	(*AudioTask[audioInfo.nFileType])(this);
}

void Mp3Tag::EditID3v1Tag() {
	ID3v1 id3v1;
	GenerateId3v1Tag(id3v1); // ����ID3v1��ǩ�ṹ
	CFile file;			     // �Զ����Ʒ�ʽ���ļ����б༭
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	
	// �ж�MP3�ļ�β���Ƿ�����ID3v1��ǩ
	file.Seek(_ID3V1_OFFSET_FROM_END, CFile::end);
	char chIdReader[_ID3V1_IDENTIFIER_LENGTH];
	memset(chIdReader, 0, sizeof(chIdReader));
	file.Read(chIdReader, _ID3V1_IDENTIFIER_LENGTH);
	int nHasNoTag = memcmp(chIdReader, _ID3V1_IDENTIFIER, _ID3V1_IDENTIFIER_LENGTH); 
	file.Seek(nHasNoTag ? 0 : _ID3V1_OFFSET_FROM_END, CFile::end);
	file.Write(&id3v1, _ID3V1_LENGTH);
	file.Close(); // �ͷ��ļ�
}
void Mp3Tag::EditID3v2Tag() {
	ID3v2 id3v2;
	GenerateId3v2Tag(id3v2); // ����ID3v2��ǩ�ṹ	
	CFile file;				 // �Զ����Ʒ�ʽ��MP3�ļ�����д��
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	
	// �ж�MP3�ļ��Ƿ�����ID3v2��ǩ
	ID3v2TagHeader id3v2TagHeaderReader;
	memset(&id3v2TagHeaderReader, 0, sizeof(id3v2TagHeaderReader));
	file.Seek(0, CFile::begin);
	file.Read(&id3v2TagHeaderReader, _ID3V2_FRAME_HEADER_LENGTH);
	int nHasNoTag = memcmp(id3v2TagHeaderReader.chIdentifier, _ID3V2_IDENTIFIER, _ID3V2_TAG_HEADER_IDENTIFIER_LENGTH); 

	UINT nSrcSize = 0; // ��¼��Ƶ���ݳ���
	if(!nHasNoTag){ // ����0�������ֽڶ���ͬ����MP3�ļ��Ѻ���ID3v2��ǩ
		// ����ԭ����ID3v2��ǩ��С�����ļ���ȡָ��������ǩĩβ������Ƶ���ݿ�ʼ��
		UINT nOldTagSize = id3v2TagHeaderReader.chTagSize[0] * 0x2000000
			+ id3v2TagHeaderReader.chTagSize[1] * 0x4000
			+ id3v2TagHeaderReader.chTagSize[2] * 0x80
			+ id3v2TagHeaderReader.chTagSize[3];
		nSrcSize = static_cast<UINT>(file.GetLength() - _ID3V2_TAG_HEADER_LENGTH - nOldTagSize);
		file.Seek(_ID3V2_TAG_HEADER_LENGTH + nOldTagSize, CFile::begin);
	}
	else{ // ���ط�0���������ֽڲ�ͬ����MP3�ļ�����ID3v2��ǩ
		nSrcSize = static_cast<UINT>(file.GetLength());
		file.Seek(0, CFile::begin);
	}
	assert(nSrcSize && L"Empty file!");
	char* srcBuffer = new char[nSrcSize]; // Delete������д���ļ�֮��
	file.Read(srcBuffer, nSrcSize);		  // ���ԭ��Ƶ����
	file.SetLength(0);					  // �����Ƶ�ļ�

	// д��ID3v2��ǩ����д��ԭ��Ƶ����
	file.Seek(0, CFile::begin);
	WriteID3v2(file, id3v2);
	file.Write(srcBuffer, nSrcSize);
	MultiDelete(srcBuffer);	
	file.Close();
}
void Mp3Tag::GenerateId3v1Tag(ID3v1 & id3v1) {
	memset(&id3v1, 0, sizeof(id3v1));// ��ʼ��ID3v1�ṹ
	memcpy(id3v1.chIdentifier, _ID3V1_IDENTIFIER, _ID3V1_IDENTIFIER_LENGTH); // ����ID3v1��ʶ��

	// ����ID3v1��ǩ��Ϣ
	UnicodeToAnsi(pAudioFile->GetTagInfo().chTitle, id3v1.chTitle, _ID3V1_TITLE_LENGTH);
	UnicodeToAnsi(pAudioFile->GetTagInfo().chArtist, id3v1.chArtist, _ID3V1_ARTIST_LENGTH);
	UnicodeToAnsi(pAudioFile->GetTagInfo().chAlbum, id3v1.chAlbum, _ID3V1_ALBUM_LENGTH);
	UnicodeToAnsi(pAudioFile->GetTagInfo().chYear, id3v1.chYear, _ID3V1_YEAR_LENGTH);
	strncpy(id3v1.chComment, _ID3V1_DEFAULT_COMMENT, _ID3V1_COMMENT_LENGTH); // ����ID3v1ע����Ϣ
	id3v1.chGenre = _ID3V1_GENRE_NOT_USE;// ����ID3v1������Ϣ����ʹ��������Ϣ
}
void Mp3Tag::GenerateId3v2Tag(ID3v2 & id3v2) {
	memset(&id3v2, 0, sizeof(id3v2)); // ��ʼ���ṹ

	// ����ID3v2��ǩ֡�ṹ���ۼ�ID3v2��ǩ�ܴ�С��������ǩͷ��10�ֽ�
	UINT nTagSize = 0;
	UINT nLengthInWords = 0;
	nLengthInWords = wcslen(pAudioFile->GetTagInfo().chTitle);
	if(nLengthInWords){ nTagSize += GenerateId3v2Frame(id3v2FrameType_title, id3v2.id3v2FrameTitle, pAudioFile->GetTagInfo().chTitle); }
	nLengthInWords = wcslen(pAudioFile->GetTagInfo().chArtist);
	if(nLengthInWords){ nTagSize += GenerateId3v2Frame(id3v2FrameType_artist, id3v2.id3v2FrameArtist, pAudioFile->GetTagInfo().chArtist); }
	nLengthInWords = wcslen(pAudioFile->GetTagInfo().chAlbum);
	if(nLengthInWords){ nTagSize += GenerateId3v2Frame(id3v2FrameType_album, id3v2.id3v2FrameAlbum, pAudioFile->GetTagInfo().chAlbum); }
	nLengthInWords = wcslen(pAudioFile->GetTagInfo().chYear);
	if(nLengthInWords){ nTagSize += GenerateId3v2Frame(id3v2FrameType_year, id3v2.id3v2FrameYear, pAudioFile->GetTagInfo().chYear); }
	nLengthInWords = wcslen(pAudioFile->GetTagInfo().chStyle);
	if(nLengthInWords){ nTagSize += GenerateId3v2Frame(id3v2FrameType_style, id3v2.id3v2FrameStyle, pAudioFile->GetTagInfo().chStyle); }
	nTagSize += GenerateId3v2CommentFrame(id3v2.id3v2FrameComment);
	if(PathFileExists(pAudioFile->GetPicInfo().chFilePath)){ nTagSize += GenerateId3v2PictureFrame(id3v2.id3v2FramePicture); }
	assert(nTagSize && L"Nothing to write in!");
	GenerateId3v2TagHeader(id3v2.id3v2TagHeader, nTagSize); // ����ID3v2��ǩͷ�ṹ
}
void Mp3Tag::GenerateId3v2TagHeader(ID3v2TagHeader & id3v2TagHeader, UINT & nTagSize) {
	memset(&id3v2TagHeader, 0, sizeof(id3v2TagHeader));   // ��ʼ��Id3v2TagHeader�ṹ
	memcpy(id3v2TagHeader.chIdentifier, _ID3V2_IDENTIFIER, 
		   _ID3V2_TAG_HEADER_IDENTIFIER_LENGTH);		  // ���ɱ�ʶ��
	memcpy(id3v2TagHeader.chVersion, _ID3V2_VERSION, 
			_ID3V2_TAG_HEADER_VERSION_LENGTH);			  // ���ɰ汾��

	// ���ɱ�ǩ��С�����㷽���Ľ����Ҫ�ο���ṹ���� http://id3.org/d3v2.3.0 ��3.1�½�
	id3v2TagHeader.chTagSize[0] = static_cast<UCHAR>(nTagSize >> 21);
	nTagSize &= 0x001FFFFF;
	id3v2TagHeader.chTagSize[1] = static_cast<UCHAR>(nTagSize >> 14);
	nTagSize &= 0x00003FFF;
	id3v2TagHeader.chTagSize[2] = static_cast<UCHAR>(nTagSize >> 7);
	nTagSize &= 0x0000007F;
	id3v2TagHeader.chTagSize[3] = static_cast<UCHAR>(nTagSize);
}
UINT Mp3Tag::GenerateId3v2Frame(const UINT & nFrameType, ID3v2Frame & id3v2Frame, const wchar_t* pContent) {
	memset(&id3v2Frame, 0, sizeof(id3v2Frame));					   // ��ʼ���ṹ
	return GenerateId3v2FrameHeader(nFrameType, id3v2Frame.header, // ������֡���ݣ��ٸ������ݳ�������֡ͷ
									GenerateId3v2FrameBody(id3v2Frame.body, pContent)); 
}
UINT Mp3Tag::GenerateId3v2CommentFrame(ID3v2CommentFrame & id3v2Comment) {
	memset(&id3v2Comment, 0, sizeof(id3v2Comment));						   // ��ʼ���ṹ
	id3v2Comment.chTextEncoding = _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE;    // �������ݱ��뷽ʽ
	memcpy(id3v2Comment.chLanguage, _ID3V2_COMMENT_FRAME_LANGUAGE_CHINESE, 
			_ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH);						   // ����ʹ������
	memcpy(id3v2Comment.chContentDescrip, _ID3V2_DEFAULT_COMMENT_DESCRIP, 
		    _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH);						   // ����Ĭ��ע������
	memcpy(id3v2Comment.chUnicodePrefix, _ID3V2_FRAME_USE_UNICODE_PREFIX, 
		   _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);					   // ����Unicode�����ַ���ǰ׺
	wcsncpy(id3v2Comment.chBody, _ID3V2_DEFAULT_COMMENT, 
			_ID3V2_FRAME_BODY_BODY_LENGTH);								   // ����ע������
	id3v2Comment.nBodyLengthInBytes = wcslen(id3v2Comment.chBody) << 1;    // ����ע�����ݳ���
	UINT nLengthInBytes = 10 + id3v2Comment.nBodyLengthInBytes;            // ����֡�����ܴ�С������1�ֽڣ�����3�ֽڣ�ע������2���֣�����Unicodeǰ׺1���֣���10�ֽڣ���������
	return GenerateId3v2FrameHeader(id3v2FrameType_comment, 
									id3v2Comment.header, nLengthInBytes);  // ����֡ͷ
}
UINT Mp3Tag::GenerateId3v2PictureFrame(ID3v2PictureFrame & id3v2Picture) {
	memset(&id3v2Picture, 0, sizeof(id3v2Picture));									  // ��ʼ���ṹ
	id3v2Picture.chTextEncoding = _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE;				  // ���ɱ��뷽ʽ
	id3v2Picture.chMIMEType = _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE[
		pAudioFile->GetPicInfo().nFileType];										  // ����MIME��Ӧ����
	id3v2Picture.nMIMETypeLengthInBytes = _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE_LENGTH[
		pAudioFile->GetPicInfo().nFileType];										  // ��¼���볤��
	id3v2Picture.chPictureType = _ID3V2_PICTURE_FRAME_PICTURE_TYPE_COVER_FRONT;       // ����ͼƬ��������
	memcpy(id3v2Picture.chDescrip, _ID3V2_DEFAULT_PICTURE_DESCRIP,
		   _ID3V2_PICTURE_FRAME_DESCRIP_LENGTH);									  // ���ɶ�ͼƬ��Ĭ������
	MultiDelete(id3v2Picture.chPictureData); // ���ͷ�ָ�루����������ݣ�
	CFile picFile;
	picFile.Open(pAudioFile->GetPicInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	id3v2Picture.nPicDataLengthInBytes = static_cast<UINT>(picFile.GetLength());  // ��¼�����ܳ���
	id3v2Picture.chPictureData = new char[id3v2Picture.nPicDataLengthInBytes];    // �����¼ͼƬ���ݵĿռ䣬DeleteҪ������ͼƬ����д���ļ�֮��
	picFile.Seek(0, CFile::begin);
	picFile.Read(id3v2Picture.chPictureData, id3v2Picture.nPicDataLengthInBytes); // ��¼ͼƬ����
	picFile.Close();	
	UINT nBodyLengthInBytes = 6 + id3v2Picture.nMIMETypeLengthInBytes 
		+ id3v2Picture.nPicDataLengthInBytes;									  // ����֡�����ܴ�С������1�ֽڣ���������1�ֽڣ�ͼƬĬ������2���֣���6�ֽڣ�����MIME����������
	return GenerateId3v2FrameHeader(id3v2FrameType_picture, id3v2Picture.header, 
									nBodyLengthInBytes);						  // ����֡ͷ
}
UINT Mp3Tag::GenerateId3v2FrameBody(ID3v2FrameBody & id3v2FrameBody, const wchar_t* pContent) {
	memset(&id3v2FrameBody, 0, sizeof(id3v2FrameBody));					     // ��ʼ���ṹ
	id3v2FrameBody.chEncode = _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE;			 // �������ݱ�����ʾ��
	memcpy(id3v2FrameBody.chUnicodePrefix, _ID3V2_FRAME_USE_UNICODE_PREFIX, 
		   _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);						 // ����Unicode�����ַ���ǰ׺
	wcsncpy(id3v2FrameBody.chBody, pContent, _ID3V2_FRAME_BODY_BODY_LENGTH); // ����֡����
	id3v2FrameBody.nBodyLengthInBytes = wcslen(id3v2FrameBody.chBody) << 1;  // ��¼֡���ݳ��ȣ�wcslen��λ���֣��ʳ�2
	return 3 + id3v2FrameBody.nBodyLengthInBytes;							 // ����֡�����ܳ��ȣ������ʶ��1�ֽ�+Unicode�ַ���ǰ׺2�ֽ�+����
}
UINT Mp3Tag::GenerateId3v2FrameHeader(const UINT & nFrameType, ID3v2FrameHeader & id3v2FrameHeader, const UINT & nBodyLengthInBytes) {
	memset(&id3v2FrameHeader, 0, sizeof(id3v2FrameHeader));						  // ��ʼ���ṹ	
	memcpy(id3v2FrameHeader.chFrameID, _ID3v2_FRAME_HEADER_ID_TYPES[nFrameType], 
		   _ID3V2_FRAME_HEADER_ID_LENGTH);										  // ����֡��ʶID	
	UintTo4BytesUCharBigEndian(nBodyLengthInBytes, id3v2FrameHeader.chFrameSize); // ����֡���ݴ�С��UINT����С����ʽ��ŵģ���ID3v2Ҫ��Ĵ�ŷ�ʽ���ơ���ˡ�������Ҫ��Size��ת
	return nBodyLengthInBytes + _ID3V2_FRAME_HEADER_LENGTH;
}

void ApeTag::EditAPEv2Tag() {
	APEv2 apev2; // ����APEv2��ǩ�ṹ
	GenerateAPEv2Tag(apev2);
	CFile file;	 // �Զ����Ʒ�ʽ��MP3�ļ�����д��
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);

	// �ж�APE�ļ��Ƿ�����APEv2��ǩ��Ĭ��APEv2��ǩ���ļ�β�����ٷ����Ƽ���APEv2��ǩ�����ļ��ײ���
	APEv2TagFooter apev2TagFooterReader;
	memset(&apev2TagFooterReader, 0, sizeof(apev2TagFooterReader));
	file.Seek(-_APEV2_TAG_HEADER_FOOTER_LENGTH, CFile::end);
	file.Read(&apev2TagFooterReader, _APEV2_TAG_HEADER_FOOTER_LENGTH);
	int nHasNoTag = memcmp(apev2TagFooterReader.chIdentifier, _APEV2_TAG_IDENTIFIER, _APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH);
	if(!nHasNoTag){ // ����0�������ֽڶ���ͬ����APE�ļ��Ѻ���APEv2��ǩ
		file.SetLength(file.GetLength() - apev2TagFooterReader.nTagSize); // ��β����Items+��ǩβ��ȥ��
		if(apev2TagFooterReader.chFlags[3] == 0x80){ // ���б�ǩͷ��ȥ�����ж��Ƿ��б�ǩͷ �ο�Flag������ http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags
			file.SetLength(file.GetLength() - _APEV2_TAG_HEADER_FOOTER_LENGTH);
		}
	}

	// ��APE�ļ�β��д��APEv2��ǩ
	file.Seek(0, CFile::end); 
	WriteAPEv2(file, apev2);
	file.Close();
}
void ApeTag::GenerateAPEv2Tag(APEv2 & apev2) {
	memset(&apev2, 0, sizeof(apev2)); // ��ʼ���ṹ
	
	// ����APEv2Items�ṹ����¼��ǩ�ܴ�С��������ǩͷ������ǩβ����¼Item����
	UINT nTagSize = 0;
	UINT nItemCount = 0; 
	if(wcslen(pAudioFile->GetTagInfo().chTitle) && (++nItemCount)){
		nTagSize += GenerateAPEv2Item(apev2ItemKeyType_title, apev2.apev2ItemTitle, pAudioFile->GetTagInfo().chTitle);
	}
	if(wcslen(pAudioFile->GetTagInfo().chArtist) && (++nItemCount)){
		nTagSize += GenerateAPEv2Item(apev2ItemKeyType_artist, apev2.apev2ItemArtist, pAudioFile->GetTagInfo().chArtist);
	}
	if(wcslen(pAudioFile->GetTagInfo().chAlbum) && (++nItemCount)){
		nTagSize += GenerateAPEv2Item(apev2ItemKeyType_album, apev2.apev2ItemAlbum, pAudioFile->GetTagInfo().chAlbum);
	}
	if(wcslen(pAudioFile->GetTagInfo().chYear) && (++nItemCount)){
		nTagSize += GenerateAPEv2Item(apev2ItemKeyType_year, apev2.apev2ItemYear, pAudioFile->GetTagInfo().chYear);
	}
	if(wcslen(pAudioFile->GetTagInfo().chStyle) && (++nItemCount)){
		nTagSize += GenerateAPEv2Item(apev2ItemKeyType_style, apev2.apev2ItemStyle, pAudioFile->GetTagInfo().chStyle);
	}
	assert(nTagSize && L"Nothing to write in!");
	if(++nItemCount){
		nTagSize += GenerateAPEv2Item(apev2ItemKeyType_comment, apev2.apev2ItemComment, _APEV2_DEFAULT_COMMENT);
	}
	nTagSize += _APEV2_TAG_HEADER_FOOTER_LENGTH;
	GenerateAPEv2TagHeaderAndFooter(apev2, nTagSize, nItemCount); // ����APEv2��ǩͷ/β�ṹ
}
UINT ApeTag::GenerateAPEv2Item(const UINT & nItemKey, APEv2Item & apev2Item, const wchar_t* pContent) {
	memset(&apev2Item, 0, sizeof(apev2Item));						    // ��ʼ���ṹ
	apev2Item.pKey = _APEV2_ITEM_KEY[nItemKey];						    // ����ItemKey����
	apev2Item.nKeyLengthInBytes = _APEV2_ITEM_KEY_LENGTH[nItemKey];     // ����ItemKey��С
	UnicodeToUTF8(pContent, apev2Item.chBody, _APEV2_ITEM_BODY_LENGTH); // ����Item���ݼ���Ӧ��С
	apev2Item.nBodyLengthInBytes = strlen(apev2Item.chBody);
	UINT nItemSize = sizeof(apev2Item.nBodyLengthInBytes) // ����Item�ܴ�С����¼Item���ݴ�С��UINT 4�ֽڣ�Flags��ռ�ֽڣ�ItemKey��ռ�ֽڣ�Item������ռ�ֽ�
		+ _APEV2_ITEM_FLAGS_LENGTH 
		+ apev2Item.nKeyLengthInBytes 
		+ apev2Item.nBodyLengthInBytes;
	return nItemSize;
}
void ApeTag::GenerateAPEv2TagHeaderAndFooter(APEv2 & apev2, const UINT & nTagSize, const UINT & nItemCount) {
	memset(&apev2.apev2TagHeader, 0, sizeof(apev2.apev2TagHeader)); // ��ʼ���ṹ
	memset(&apev2.apev2TagFooter, 0, sizeof(apev2.apev2TagFooter));
	memcpy(apev2.apev2TagHeader.chIdentifier, _APEV2_TAG_IDENTIFIER, 
			_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH);            // ���ɱ�ǩ��ʶ��
	memcpy(apev2.apev2TagFooter.chIdentifier, _APEV2_TAG_IDENTIFIER,
			_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH);
	apev2.apev2TagHeader.nVersion = _APEV2_TAG_VERSION_2_0;	        // ���ɰ汾��
	apev2.apev2TagFooter.nVersion = _APEV2_TAG_VERSION_2_0;
	apev2.apev2TagHeader.nTagSize = nTagSize;					    // ���ɱ�ǩ�ܴ�С��������ǩͷ������ǩβ��
	apev2.apev2TagFooter.nTagSize = nTagSize;
	apev2.apev2TagHeader.nItemCount = nItemCount;					// ����ItemCount
	apev2.apev2TagFooter.nItemCount = nItemCount;
	memcpy(apev2.apev2TagHeader.chFlags, _APEV2_TAG_FLAGS_HEADER, 	// ����ͷβ��ʶ��
		   _APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH);
	memcpy(apev2.apev2TagFooter.chFlags, _APEV2_TAG_FLAGS_FOOTER, 
		   _APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH);
}

void FlacTag::EditFLACTag() {
	FLAC flac;												 // ��ʼ��FLAC�ļ�ͷ�ṹ
	memset(&flac, 0, sizeof(flac));
	GenerateFLACVorbisComment(flac.vorbisComment);			 // ����FLAC��ǩ��
	if(PathFileExists(pAudioFile->GetPicInfo().chFilePath)){ // ����FLACͼƬ��
		GenerateFLACPicture(flac.picture);
	}			  
	CFile file;												 // �Զ����Ʒ�ʽ��MP3�ļ�����д��
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	file.Seek(0, CFile::begin);
	file.Read(flac.chIdentifier, _FLAC_IDENTIFIER_LENGTH);   // ��ȡ��ʶ��
	assert(!memcmp(flac.chIdentifier, _FLAC_IDENTIFIER, _FLAC_IDENTIFIER_LENGTH)); // ����ʶ�����ԣ����ǷǷ�FLAC�ļ�
	FLACBlockHeader headerReader;
	while(1){ // ���������ȡ����Ϣ������Ϣ��¼��������
		memset(&headerReader, 0, sizeof(headerReader));
		file.Read(&headerReader, _FLAC_BLOCK_HEADER_LENGTH);			   // ��ȡ��ͷ���ж��Ƿ���FLAC�ļ������һ����
		UINT nBlockType = headerReader.chType & _FLAC_GET_BLOCK_TYPE_MASK; // ȡ��������
		UINT nBlockSize = headerReader.chSize[0] * 0x10000			       // ��������ݳ���
			+ headerReader.chSize[1] * 0x100
			+ headerReader.chSize[2];
		if(nBlockType != flacBlockType_vorbisComment && nBlockType != flacBlockType_picture){ // ���˱�ǩ��ͼƬ��Ϣ���⣬����¼ԭ�е���Ϣ
			flac.BlockList[nBlockType].header.chType = static_cast<UCHAR>(nBlockType); // ��¼�����ͣ���ʱ���λ����0����ʾ�������һ����
			memcpy(flac.BlockList[nBlockType].header.chSize, headerReader.chSize, 
				   _FLAC_BLOCK_HEADER_SIZE_LENGTH);									   // ����ԭ�����ݴ�С
			flac.BlockList[nBlockType].body.nBodyLengthInBytes = nBlockSize;		   // ��¼�����ݳ���
			MultiDelete(flac.BlockList[nBlockType].body.chBody);				       // ���ͷ�ָ��ԭ��ָ�������
			flac.BlockList[nBlockType].body.chBody = new char[nBlockSize];			   // Delete������д���ļ����֮��
			file.Read(flac.BlockList[nBlockType].body.chBody, nBlockSize);
		}
		else{ // ����¼ԭ�б�ǩ��ͼƬ��Ϣ������
			file.Seek(nBlockSize, CFile::current);
		}
		if(headerReader.chType & _FLAC_IS_LAST_BLOCK_MASK) { // �����һ���飬����Ҫ�˳�ѭ��
			flac.BlockList[flacBlockType_padding].header.chType = _FLAC_PADDING_AS_LAST_BLOCK; // ��Padding����Ϊ���һ����
			if(!flac.BlockList[flacBlockType_padding].body.nBodyLengthInBytes){ // ���ԭ����FLAC�ļ���û��Padding�������
				flac.BlockList[flacBlockType_padding].body.nBodyLengthInBytes = _FLAC_DEFAULT_PADDING_LENGTH; 
				memcpy(flac.BlockList[flacBlockType_padding].header.chSize, _FLAC_DEFAULT_PADDING_SIZE, 
					   _FLAC_BLOCK_HEADER_SIZE_LENGTH);							// ���ɿ��С
				MultiDelete(flac.BlockList[flacBlockType_padding].body.chBody); // �ͷ�ԭ�����ݣ�����У�
				flac.BlockList[flacBlockType_padding].body.chBody = new char[_FLAC_DEFAULT_PADDING_LENGTH]; // Delete������д���ļ�֮��
				memset(flac.BlockList[flacBlockType_padding].body.chBody, 0, _FLAC_DEFAULT_PADDING_LENGTH);
			}
			break; // �˳�ѭ��
		}
	}
	UINT nSrcSize = static_cast<UINT>(file.GetLength() - file.GetPosition()); // ����ԭ��Ƶ���ݳ���
	char* pSrcBuffer = new char[nSrcSize]; // Delete������д���ļ����֮��
	file.Read(pSrcBuffer, nSrcSize);	   // ȡ��ԭ��Ƶ����
	file.SetLength(0);					   // ���FLAC�ļ�

	// д���ļ�ͷ����д��ԭ��Ƶ����
	file.Seek(0, CFile::begin);
	WriteFLAC(file, flac);
	file.Write(pSrcBuffer, nSrcSize);
	MultiDelete(pSrcBuffer);
	file.Close();
}
void FlacTag::GenerateFLACVorbisComment(FLACVorbisComment & vorbisComment) {
	memset(&vorbisComment, 0, sizeof(vorbisComment));								   // ��ʼ���ṹ
	vorbisComment.nDescripLengthInBytes = _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH; // ���ɱ�ǩ����������
	memcpy(vorbisComment.chDescrip, _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP, 
			_FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH);							   // ���ɱ�ǩ������

	// ���ɸ�����ǩ֡����¼֡�ܸ�������������ݴ�С��������ͷ��
	vorbisComment.nFrameCount = 0;
	UINT nLengthInBytes = 0;
	if(wcslen(pAudioFile->GetTagInfo().chTitle) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_title, vorbisComment.title, pAudioFile->GetTagInfo().chTitle); }
	if(wcslen(pAudioFile->GetTagInfo().chArtist) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_artist, vorbisComment.artist, pAudioFile->GetTagInfo().chArtist); }
	if(wcslen(pAudioFile->GetTagInfo().chAlbum) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_album, vorbisComment.album, pAudioFile->GetTagInfo().chAlbum); }
	if(wcslen(pAudioFile->GetTagInfo().chYear) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_year, vorbisComment.year, pAudioFile->GetTagInfo().chYear); }
	if(wcslen(pAudioFile->GetTagInfo().chStyle) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_style, vorbisComment.style, pAudioFile->GetTagInfo().chStyle); }
	assert(nLengthInBytes && L��Nothing to write in!��); 
	if(++vorbisComment.nFrameCount){ nLengthInBytes += GenerateFLACFrame(flacFrameType_comment, vorbisComment.comment, _FLAC_DEFAULT_COMMENT); }
	vorbisComment.header.chType = _FLAC_BLOCK_VORBIS_COMMENT_TYPE;		     // ���ɿ�������Ϣ
	nLengthInBytes += _FLAC_BLOCK_VORBIS_COMMENT_BASE_LENGTH;				 // �������г���֡����Ļ�������
	nLengthInBytes += vorbisComment.nFrameCount * 4;						 // ÿ��֡ǰ���������ݳ��ȵ�����4�ֽ�
	UintTo3BytesUCharBigEndian(nLengthInBytes, vorbisComment.header.chSize); // ���ɿ����ݴ�С��������ͷ��
}
void FlacTag::GenerateFLACPicture(FLACPicture & picture) {
	memset(&picture, 0, sizeof(picture));		    // ��ʼ���ṹ
	memcpy(picture.chPicType, _FLAC_BLOCK_PICTURE_PIC_TYPE_COVER_FRONT, 
		   _FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH);    // ����ͼƬ��������
	picture.nMIMETypeLengthInBytes = _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE_LENGTH[pAudioFile->GetPicInfo().nFileType]; 
    UintTo4BytesUCharBigEndian(picture.nMIMETypeLengthInBytes, picture.chMIMESize);				 // ����MIME���ͷ���С��UINT����С����ʽ��ŵģ���FLACҪ��Ĵ�ŷ�ʽ���ơ���ˡ�������Ҫ��Size��ת
	picture.chMIMEType = _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE[pAudioFile->GetPicInfo().nFileType]; // ����MIME���ͷ�
	
	// ����ͼƬ����
	MultiDelete(picture.chPicData); // ���ͷ�ָ�루����������ݣ�
	CFile picFile;
	picFile.Open(pAudioFile->GetPicInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	picture.nPicDataLengthInBytes = static_cast<UINT>(picFile.GetLength());
	picture.chPicData = new char[picture.nPicDataLengthInBytes]; // DeleteҪ������ͼƬ����д���ļ�֮��
	picFile.Seek(0, CFile::begin);
	picFile.Read(picture.chPicData, picture.nPicDataLengthInBytes);
	picFile.Close();
	UintTo4BytesUCharBigEndian(picture.nPicDataLengthInBytes, picture.chPicDataSize); // ����ͼƬ���ݴ�С��UINT����С����ʽ��ŵģ���FLACҪ��Ĵ�ŷ�ʽ���ơ���ˡ�������Ҫ��Size��ת
	picture.header.chType = _FLAC_BLOCK_PICTURE_TYPE;     // ���ɿ�������Ϣ
	UINT nLengthInBytes = _FLAC_BLOCK_PICTURE_BASE_LENGTH // ��������ݴ�С��������ͷ��
		+ picture.nMIMETypeLengthInBytes
		+ picture.nPicDataLengthInBytes;
	UintTo3BytesUCharBigEndian(nLengthInBytes, picture.header.chSize); // ���ɿ����ݴ�С��������ͷ��
}
UINT FlacTag::GenerateFLACFrame(const UINT & nFrameType, FLACFrame & frame, const wchar_t* pContent) {
	memset(&frame, 0, sizeof(frame)); // ��ʼ���ṹ
	strncpy(frame.chBody, _FLAC_FRAME_TYPE_IDENTIFIER[nFrameType], _FLAC_FRAME_BODY_LENGTH); // ����֡�������ͱ�ʶ
	char buffer[_FLAC_FRAME_BODY_LENGTH];
	UnicodeToUTF8(pContent, buffer, _FLAC_FRAME_BODY_LENGTH); // ����֡����
	strncat(frame.chBody, buffer, _FLAC_FRAME_BODY_LENGTH - strlen(frame.chBody));
	frame.nBodyInBytes = strlen(frame.chBody); // ��¼֡���ݳ���
	return frame.nBodyInBytes;
}

