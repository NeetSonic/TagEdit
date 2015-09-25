//-----------------
// 音频文件类 实现文件
// 2015.1.4
// Fairchild_Lhy
//-----------------
#include "AudioFile.h"

//----------定义全局变量----------
// ID3v1标签标识符
const char _ID3V1_IDENTIFIER[_ID3V1_IDENTIFIER_LENGTH] = {'T', 'A', 'G'};

// ID3v2标签头标识符
const char _ID3V2_IDENTIFIER[_ID3V2_TAG_HEADER_IDENTIFIER_LENGTH] = {'I', 'D', '3'};

// ID3v1标签默认注释信息
const char* _ID3V1_DEFAULT_COMMENT = "本标签由TagEdit v1.0生成";

// ID3v2标签默认注释信息
const wchar_t* _ID3V2_DEFAULT_COMMENT = _T("本标签由TagEdit v1.0生成");

// ID3v2标签版本号
const char _ID3V2_VERSION[_ID3V2_TAG_HEADER_VERSION_LENGTH] = {0x03, 0x00}; // 代表ID3v2.3.0版本

// ID3v2帧内容使用Unicode编码写入之前需要添加的前缀
const UCHAR _ID3V2_FRAME_USE_UNICODE_PREFIX[_ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH] = {0xFF, 0xFE};

// ID3v2帧头标识ID种类，序号要与enum ID3v2FrameType对应
const char _ID3v2_FRAME_HEADER_ID_TYPES[_ID3V2_FRAME_TYPE_COUNT][_ID3V2_FRAME_HEADER_ID_LENGTH] = {
		{'T', 'I', 'T', '2'}, {'T', 'P', 'E', '1'}, {'T', 'A', 'L', 'B'}, {'T', 'Y', 'E', 'R'},
		{'T', 'C', 'O', 'N'}, {'C', 'O', 'M', 'M'}, {'A', 'P', 'I', 'C'}
};

// ID3v2注释帧内容使用的语种
const char _ID3V2_COMMENT_FRAME_LANGUAGE_CHINESE[_ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH] = {'c', 'h', 'i'};

// ID3v2注释帧默认描述
const UCHAR _ID3V2_DEFAULT_COMMENT_DESCRIP[_ID3V2_COMMENT_FRAME_DESCRIP_LENGTH] = {0xFF, 0xFE, 0x00, 0x00};

// ID3v2图片帧使用的MIME类型对应的代码，顺序号要与enum fileType里的顺序一致，前面用空字符串占位
const char* _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE[_FILE_TYPE_COUNT] = {
	NULL, NULL, NULL, NULL, NULL, "image/png", "image/jpeg"
};

// ID3v2图片帧使用的MIME类型对应的代码字符串长度，包含结尾符0x00（单位：字节），顺序号要与enum fileType里的顺序一致，前面用0占位
const UINT _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE_LENGTH[_FILE_TYPE_COUNT] = {
	0, 0, 0, 0, 0, 10, 11
};

// APEv2标签版本号2.0
const UINT _APEV2_TAG_VERSION_2_0 = 2000;

// APEv2标签头/尾标识，详细设置参考 http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags
const UCHAR _APEV2_TAG_FLAGS_HEADER[_APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH] = {0x00, 0x00, 0x00, 0xA0};
const UCHAR _APEV2_TAG_FLAGS_FOOTER[_APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH] = {0x00, 0x00, 0x00, 0x80};

// APEv2标签标识符
const char _APEV2_TAG_IDENTIFIER[_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH] = {
	'A', 'P', 'E', 'T', 'A', 'G', 'E', 'X'
};

// APEv2ItemKey各类型对应的标识，序号要与enum APEv2ItemKeyType对应
const char* _APEV2_ITEM_KEY[_APEV2_ITEM_KEY_TYPE_COUNT] = {
	"Title", "Artist", "Album", "Year", "Genre", "Comment"
};

// APEv2ItemKey各类型对应的标识的长度，含字符串尾的0x00字节，序号要与enum APEv2ItemKeyType对应
const UINT _APEV2_ITEM_KEY_LENGTH[_APEV2_ITEM_KEY_TYPE_COUNT] = {
	6, 7, 6, 5, 6, 8
};

// FLAC文件头部标识（每个FLAC文件一开始都必须有）
const char* _FLAC_IDENTIFIER = "fLaC";

// FLAC文件中最后一个块的判断掩码
const UCHAR _FLAC_IS_LAST_BLOCK_MASK = 0x80;

// FLAC文件取出块类型的掩码
const UCHAR _FLAC_GET_BLOCK_TYPE_MASK = 0x7F;

// FLAC文件Padding块作为最后一个块的Block类型字节
const UCHAR _FLAC_PADDING_AS_LAST_BLOCK = 0x81;

// FLAC文件标签块类型字节
const UCHAR _FLAC_BLOCK_VORBIS_COMMENT_TYPE = 0x04;

// FLAC文件图片块类型字节
const UCHAR _FLAC_BLOCK_PICTURE_TYPE = 0x06;

// FLAC文件默认Paddind大小，整个块必须是8的倍数，块头4字节，内容使用1KB - 4，FLAC规定大端存放即00 03 FC
const UCHAR _FLAC_DEFAULT_PADDING_SIZE[_FLAC_BLOCK_HEADER_SIZE_LENGTH] = {0x00, 0x03, 0xFC};

// FLAC文件标签块默认描述
const char* _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP = "Xiph.Org libVorbis I 20020717";

// FLAC文件图片块使用的图片类型描述字节（封面front）
const char _FLAC_BLOCK_PICTURE_PIC_TYPE_COVER_FRONT[_FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH] = {0x00, 0x00, 0x00, 0x03};

// FLAC文件图片块使用的MIME类型对应的代码字符串长度，不含结尾符0x00（单位：字节），顺序号要与enum fileType里的顺序一致，前面用0占位
const UINT _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE_LENGTH[_FILE_TYPE_COUNT] = {
	0, 0, 0, 0, 0, 9, 10
};

// FLAC文件标签块帧类型标识，顺序要与enum FLACFrameType一致
const char* _FLAC_FRAME_TYPE_IDENTIFIER[_FLAC_FRAME_TYPE_COUNT] {
	"TITLE=", "ARTIST=", "ALBUM=", "DATE=", "GENRE=", "DESCRIPTION="
};

// 向MP3文件写入ID3v2普通帧的方式
#define WriteID3v2Frame(file, frame)\
	if(frame.body.nBodyLengthInBytes){\
		file.Write(&frame.header, _ID3V2_FRAME_HEADER_LENGTH);\
		file.Write(&frame.body.chEncode, sizeof(frame.body.chEncode));\
		file.Write(frame.body.chUnicodePrefix, _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);\
		file.Write(frame.body.chBody, frame.body.nBodyLengthInBytes);\
	}

// 向MP3文件写入ID3v2注释帧的方式
#define WriteID3v2CommentFrame(file, frame)\
	file.Write(&frame.header, _ID3V2_FRAME_HEADER_LENGTH);\
	file.Write(&frame.chTextEncoding, sizeof(frame.chTextEncoding));\
	file.Write(frame.chLanguage, _ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH);\
	file.Write(frame.chContentDescrip, _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH);\
	file.Write(frame.chUnicodePrefix, _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);\
	file.Write(frame.chBody, frame.nBodyLengthInBytes)

// 向MP3文件写入ID3v2图片帧的方式
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

// 向MP3文件写入ID3v2标签的方式
#define WriteID3v2(file, id3v2)\
	file.Write(&id3v2.id3v2TagHeader, _ID3V2_TAG_HEADER_LENGTH);\
	WriteID3v2Frame(file, id3v2.id3v2FrameTitle);\
	WriteID3v2Frame(file, id3v2.id3v2FrameArtist);\
	WriteID3v2Frame(file, id3v2.id3v2FrameAlbum);\
	WriteID3v2Frame(file, id3v2.id3v2FrameYear);\
	WriteID3v2Frame(file, id3v2.id3v2FrameStyle);\
	WriteID3v2CommentFrame(file, id3v2.id3v2FrameComment);\
	WriteID3v2PictureFrame(file, id3v2.id3v2FramePicture)

// 向APE文件写入APEv2Item的方式
#define WriteAPEv2Item(file, item)\
	if(item.nBodyLengthInBytes){\
		file.Write(&item.nBodyLengthInBytes, sizeof(item.nBodyLengthInBytes));\
		file.Write(item.chFlags, _APEV2_ITEM_FLAGS_LENGTH);\
		file.Write(item.pKey, item.nKeyLengthInBytes);\
		file.Write(item.chBody, item.nBodyLengthInBytes);\
	}

// 向APE文件写入APEv2标签的方式
#define WriteAPEv2(file, apev2)\
	file.Write(&apev2.apev2TagHeader, _APEV2_TAG_HEADER_FOOTER_LENGTH);\
	WriteAPEv2Item(file, apev2.apev2ItemTitle);\
	WriteAPEv2Item(file, apev2.apev2ItemArtist);\
	WriteAPEv2Item(file, apev2.apev2ItemAlbum);\
	WriteAPEv2Item(file, apev2.apev2ItemYear);\
	WriteAPEv2Item(file, apev2.apev2ItemStyle);\
	WriteAPEv2Item(file, apev2.apev2ItemComment);\
	file.Write(&apev2.apev2TagFooter, _APEV2_TAG_HEADER_FOOTER_LENGTH)

// 向FLAC文件写入一般块的方式
#define WriteFLACBlock(file, block)\
	if(block.body.nBodyLengthInBytes){\
		file.Write(&block.header, _FLAC_BLOCK_HEADER_LENGTH);\
		file.Write(block.body.chBody, block.body.nBodyLengthInBytes);\
		MultiDelete(block.body.chBody);\
	}

// 向FLAC文件写入标签帧的方式
#define WriteFLACFrame(file, frame)\
	if(frame.nBodyInBytes){\
		file.Write(&frame.nBodyInBytes, sizeof(frame.nBodyInBytes));\
		file.Write(frame.chBody, frame.nBodyInBytes);\
	}

// 向FLAC文件写入标签块的方式
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

// 向FLAC文件写入图片块的方式
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

// 向FLAC文件写入FLAC文件头的方式
#define WriteFLAC(file, flac)\
	file.Write(flac.chIdentifier, _FLAC_IDENTIFIER_LENGTH);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_streamInfo]);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_application]);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_seekTable]);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_cueSheet]);\
	WriteFLACVorbisComment(file, flac.vorbisComment);\
	WriteFLACPicture(file, flac.picture);\
	WriteFLACBlock(file, flac.BlockList[flacBlockType_padding])

// 通用功能函数
inline void UnicodeToAnsi(const wchar_t* srcStr, char* destStr, const UINT & nMaxBytesToConvert) {
	int nLen = WideCharToMultiByte(CP_ACP, 0, srcStr, -1, NULL, 0, NULL, NULL); // 得到转换后的Ansi字符串长度，包含结尾符\0
	if(nLen){
		char* p = new char[nLen];
		WideCharToMultiByte(CP_ACP, 0, srcStr, -1, p, nLen, NULL, NULL); // 转换
		strncpy(destStr, p, nMaxBytesToConvert); // 在nMaxBytesToConvert范围内遇到\0则后面都填充\0
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
	// 第一个循环确保cstr长度如果不为0，则至少首位不是字符' '
	while(cstr.GetAt(0) == ' '){ cstr = cstr.Right(cstr.GetLength() - 1); }
	while((cstr.GetLength() > 1) && (cstr.GetAt(cstr.GetLength() - 1) == ' ')){ cstr = cstr.Left(cstr.GetLength() - 1); }
}

// 任务处理函数
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

// 类函数定义
BOOL AudioFile::Open(const wchar_t* filePath) {
	if(!PathFileExists(filePath)) return FALSE; // 判断文件是否存在，不存在即返回FALSE提示失败
	GenerateFileInfo(filePath, audioInfo);      // 生成FileInfo结构
	ReadTagInfoFromFileName();				    // 从文件名中获取标签信息并填入TagInfo结构
	return TRUE;
}
BOOL AudioFile::AddCoverPicture(const wchar_t* picPath) {
	if(!PathFileExists(picPath)) return FALSE; // 判断文件是否存在，不存在即返回FALSE提示失败
	GenerateFileInfo(picPath, picInfo);        // 生成FileInfo结构
	return TRUE;
}
void AudioFile::ReadTagInfoFromFileName() {
	CString csFileName(audioInfo.chFileName);
	CString csTitle, csArtist, csAlbum, csYear, csStyle;
	int		nCurrFlag;						  // 当前分隔符的位置
	int		nLastFlag = 0;					  // 上一次分隔符的位置
	int		nCount;						      // 存放截取字符的个数
	int		nLength = csFileName.GetLength(); // 文件名总长度

	//----------确定标题----------
	nCurrFlag = csFileName.Find('-', nLastFlag);
	if(nCurrFlag == -1){
		//AddErrCode(NAME_ERR_01); // 文件名没有分隔符,可能造成没有歌手信息
		csTitle = csFileName;
		DropSpace(csTitle);
		wcsncpy(tagInfo.chTitle, csTitle, _MAX_TITLE_LENGTH); // 用wcsncpy，因为lstrcpynW第三个参数规定的字的尾部自动改为0x0000
		return;
	}
	else{
		nCount = nCurrFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_02); } // 文件名分隔符出现在首位,可能造成没有标题信息
		csTitle = csFileName.Left(nCount);
		DropSpace(csTitle);
		wcsncpy(tagInfo.chTitle, csTitle, _MAX_TITLE_LENGTH);
	}

	//----------确定艺术家----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	if(nCurrFlag == -1){ // 原文件名格式为"歌曲名-歌手名"
		nCount = nLength - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_03); } // 文件名以分隔符结尾可能造成没有歌手信息
		csArtist = csFileName.Right(nCount);
		DropSpace(csArtist);
		wcsncpy(tagInfo.chArtist, csArtist, _MAX_ARTIST_LENGTH);
		return;
	}
	else{ // nCurrFlag != -1, 表示后面还有专辑信息
		nCount = nCurrFlag - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_04); } // 文件名出现2个分隔符相连的情况,可能造成没有歌手信息
		csArtist = csFileName.Mid(nLastFlag, nCount);
		DropSpace(csArtist);
		wcsncpy(tagInfo.chArtist, csArtist, _MAX_ARTIST_LENGTH);
	}

	//----------确定专辑----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	if(nCurrFlag == -1){ // 原文件名格式为"歌曲名-歌手名-专辑"
		nCount = nLength - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_05); } // 文件名以分隔符结尾可能造成没有专辑信息
		csAlbum = csFileName.Right(nCount);
		DropSpace(csAlbum);
		wcsncpy(tagInfo.chAlbum, csAlbum, _MAX_ALBUM_LENGTH);
		return;
	}
	else{ // nCurrFlag != -1, 表示后面还有年份信息
		nCount = nCurrFlag - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_06); } // 文件名出现2个分隔符相连的情况,可能造成没有专辑信息
		csAlbum = csFileName.Mid(nLastFlag, nCount);
		DropSpace(csAlbum);
		wcsncpy(tagInfo.chAlbum, csAlbum, _MAX_ALBUM_LENGTH);
	}

	//----------确定年份----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	if(nCurrFlag == -1){ // 原文件名格式为"歌曲名-歌手名-专辑-年份"
		nCount = nLength - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_07); } // 文件名以分隔符结尾可能造成没有年份信息
		csYear = csFileName.Right(nCount);
		DropSpace(csYear);
		wcsncpy(tagInfo.chYear, csYear, _MAX_ALBUM_LENGTH);
		return;
	}
	else{ // nCurrFlag != -1, 表示后面还有流派信息
		nCount = nCurrFlag - nLastFlag;
		//if(nCount == 0){ AddErrCode(NAME_ERR_08); } // 文件名出现2个分隔符相连的情况,可能造成没有年份信息
		csYear = csFileName.Mid(nLastFlag, nCount);
		DropSpace(csYear);
		wcsncpy(tagInfo.chYear, csYear, _MAX_ALBUM_LENGTH);
	}

	//----------确定流派----------
	nLastFlag = nCurrFlag;
	nCurrFlag = csFileName.Find('-', ++nLastFlag);
	//if(nCurrFlag != -1) { AddErrCode(NAME_ERR_09); } // 文件名中有多余的分隔符,可能造成所有标签信息不正确
	nCount = nLength - nLastFlag;
	//if(nCount == 0){ AddErrCode(NAME_ERR_10); } // 文件名以分隔符结尾可能造成没有流派信息
	csStyle = csFileName.Right(nCount);
	DropSpace(csStyle);
	wcsncpy(tagInfo.chStyle, csStyle, _MAX_ALBUM_LENGTH);
}
void AudioFile::GenerateFileInfo(const wchar_t* filePath, FileInfo & info) {
	wcsncpy(info.chFilePath, filePath, _MAX_PATH); // 在对应变量内存放有效的文件路径
	CString cs(filePath);
	CString csName, csType;
	cs = cs.Right(cs.GetLength() - cs.ReverseFind('\\') - 1);
	int nFlag = cs.ReverseFind('.');
	csName = cs.Left(nFlag);
	csType = cs.Right(cs.GetLength() - nFlag - 1);
	csType = csType.MakeLower();
	wcsncpy(info.chFileName, csName, _MAX_PATH);
	wcsncpy(info.chFileType, csType, _MAX_FILE_TYPE_LENGTH);

	// 为后缀名编号，易于识别
	// 音频文件
	if(!wcsncmp(info.chFileType, _T("mp3"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_mp3; }
	else if(!wcsncmp(info.chFileType, _T("ape"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_ape; }
	else if(!wcsncmp(info.chFileType, _T("flac"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_flac; }
	else if(!wcsncmp(info.chFileType, _T("wav"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_wav; }
	// 图片文件
	else if(!wcsncmp(info.chFileType, _T("png"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_png; }
	else if(!wcsncmp(info.chFileType, _T("jpg"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_jpg_jpeg; }
	else if(!wcsncmp(info.chFileType, _T("jpeg"), _MAX_FILE_TYPE_LENGTH)){ info.nFileType = fileType_jpg_jpeg; }
}
void AudioFile::StartEdit() {
	(*AudioTask[audioInfo.nFileType])(this);
}

void Mp3Tag::EditID3v1Tag() {
	ID3v1 id3v1;
	GenerateId3v1Tag(id3v1); // 生成ID3v1标签结构
	CFile file;			     // 以二进制方式打开文件进行编辑
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	
	// 判断MP3文件尾部是否已有ID3v1标签
	file.Seek(_ID3V1_OFFSET_FROM_END, CFile::end);
	char chIdReader[_ID3V1_IDENTIFIER_LENGTH];
	memset(chIdReader, 0, sizeof(chIdReader));
	file.Read(chIdReader, _ID3V1_IDENTIFIER_LENGTH);
	int nHasNoTag = memcmp(chIdReader, _ID3V1_IDENTIFIER, _ID3V1_IDENTIFIER_LENGTH); 
	file.Seek(nHasNoTag ? 0 : _ID3V1_OFFSET_FROM_END, CFile::end);
	file.Write(&id3v1, _ID3V1_LENGTH);
	file.Close(); // 释放文件
}
void Mp3Tag::EditID3v2Tag() {
	ID3v2 id3v2;
	GenerateId3v2Tag(id3v2); // 生成ID3v2标签结构	
	CFile file;				 // 以二进制方式打开MP3文件进行写入
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	
	// 判断MP3文件是否已有ID3v2标签
	ID3v2TagHeader id3v2TagHeaderReader;
	memset(&id3v2TagHeaderReader, 0, sizeof(id3v2TagHeaderReader));
	file.Seek(0, CFile::begin);
	file.Read(&id3v2TagHeaderReader, _ID3V2_FRAME_HEADER_LENGTH);
	int nHasNoTag = memcmp(id3v2TagHeaderReader.chIdentifier, _ID3V2_IDENTIFIER, _ID3V2_TAG_HEADER_IDENTIFIER_LENGTH); 

	UINT nSrcSize = 0; // 记录音频内容长度
	if(!nHasNoTag){ // 返回0，代表字节都相同，即MP3文件已含有ID3v2标签
		// 计算原来的ID3v2标签大小，将文件读取指针移至标签末尾，即音频内容开始处
		UINT nOldTagSize = id3v2TagHeaderReader.chTagSize[0] * 0x2000000
			+ id3v2TagHeaderReader.chTagSize[1] * 0x4000
			+ id3v2TagHeaderReader.chTagSize[2] * 0x80
			+ id3v2TagHeaderReader.chTagSize[3];
		nSrcSize = static_cast<UINT>(file.GetLength() - _ID3V2_TAG_HEADER_LENGTH - nOldTagSize);
		file.Seek(_ID3V2_TAG_HEADER_LENGTH + nOldTagSize, CFile::begin);
	}
	else{ // 返回非0，代表有字节不同，即MP3文件不含ID3v2标签
		nSrcSize = static_cast<UINT>(file.GetLength());
		file.Seek(0, CFile::begin);
	}
	assert(nSrcSize && L"Empty file!");
	char* srcBuffer = new char[nSrcSize]; // Delete紧跟在写入文件之后
	file.Read(srcBuffer, nSrcSize);		  // 存放原音频内容
	file.SetLength(0);					  // 清空音频文件

	// 写入ID3v2标签，再写入原音频内容
	file.Seek(0, CFile::begin);
	WriteID3v2(file, id3v2);
	file.Write(srcBuffer, nSrcSize);
	MultiDelete(srcBuffer);	
	file.Close();
}
void Mp3Tag::GenerateId3v1Tag(ID3v1 & id3v1) {
	memset(&id3v1, 0, sizeof(id3v1));// 初始化ID3v1结构
	memcpy(id3v1.chIdentifier, _ID3V1_IDENTIFIER, _ID3V1_IDENTIFIER_LENGTH); // 生成ID3v1标识符

	// 生成ID3v1标签信息
	UnicodeToAnsi(pAudioFile->GetTagInfo().chTitle, id3v1.chTitle, _ID3V1_TITLE_LENGTH);
	UnicodeToAnsi(pAudioFile->GetTagInfo().chArtist, id3v1.chArtist, _ID3V1_ARTIST_LENGTH);
	UnicodeToAnsi(pAudioFile->GetTagInfo().chAlbum, id3v1.chAlbum, _ID3V1_ALBUM_LENGTH);
	UnicodeToAnsi(pAudioFile->GetTagInfo().chYear, id3v1.chYear, _ID3V1_YEAR_LENGTH);
	strncpy(id3v1.chComment, _ID3V1_DEFAULT_COMMENT, _ID3V1_COMMENT_LENGTH); // 生成ID3v1注释信息
	id3v1.chGenre = _ID3V1_GENRE_NOT_USE;// 生成ID3v1流派信息，不使用流派信息
}
void Mp3Tag::GenerateId3v2Tag(ID3v2 & id3v2) {
	memset(&id3v2, 0, sizeof(id3v2)); // 初始化结构

	// 生成ID3v2标签帧结构，累加ID3v2标签总大小，不含标签头的10字节
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
	GenerateId3v2TagHeader(id3v2.id3v2TagHeader, nTagSize); // 生成ID3v2标签头结构
}
void Mp3Tag::GenerateId3v2TagHeader(ID3v2TagHeader & id3v2TagHeader, UINT & nTagSize) {
	memset(&id3v2TagHeader, 0, sizeof(id3v2TagHeader));   // 初始化Id3v2TagHeader结构
	memcpy(id3v2TagHeader.chIdentifier, _ID3V2_IDENTIFIER, 
		   _ID3V2_TAG_HEADER_IDENTIFIER_LENGTH);		  // 生成标识符
	memcpy(id3v2TagHeader.chVersion, _ID3V2_VERSION, 
			_ID3V2_TAG_HEADER_VERSION_LENGTH);			  // 生成版本号

	// 生成标签大小，计算方法的解读需要参考其结构定义 http://id3.org/d3v2.3.0 第3.1章节
	id3v2TagHeader.chTagSize[0] = static_cast<UCHAR>(nTagSize >> 21);
	nTagSize &= 0x001FFFFF;
	id3v2TagHeader.chTagSize[1] = static_cast<UCHAR>(nTagSize >> 14);
	nTagSize &= 0x00003FFF;
	id3v2TagHeader.chTagSize[2] = static_cast<UCHAR>(nTagSize >> 7);
	nTagSize &= 0x0000007F;
	id3v2TagHeader.chTagSize[3] = static_cast<UCHAR>(nTagSize);
}
UINT Mp3Tag::GenerateId3v2Frame(const UINT & nFrameType, ID3v2Frame & id3v2Frame, const wchar_t* pContent) {
	memset(&id3v2Frame, 0, sizeof(id3v2Frame));					   // 初始化结构
	return GenerateId3v2FrameHeader(nFrameType, id3v2Frame.header, // 先生成帧内容，再根据内容长度生成帧头
									GenerateId3v2FrameBody(id3v2Frame.body, pContent)); 
}
UINT Mp3Tag::GenerateId3v2CommentFrame(ID3v2CommentFrame & id3v2Comment) {
	memset(&id3v2Comment, 0, sizeof(id3v2Comment));						   // 初始化结构
	id3v2Comment.chTextEncoding = _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE;    // 生成内容编码方式
	memcpy(id3v2Comment.chLanguage, _ID3V2_COMMENT_FRAME_LANGUAGE_CHINESE, 
			_ID3V2_COMMENT_FRAME_LANGUAGE_LENGTH);						   // 生成使用语种
	memcpy(id3v2Comment.chContentDescrip, _ID3V2_DEFAULT_COMMENT_DESCRIP, 
		    _ID3V2_COMMENT_FRAME_DESCRIP_LENGTH);						   // 生成默认注释描述
	memcpy(id3v2Comment.chUnicodePrefix, _ID3V2_FRAME_USE_UNICODE_PREFIX, 
		   _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);					   // 生成Unicode编码字符串前缀
	wcsncpy(id3v2Comment.chBody, _ID3V2_DEFAULT_COMMENT, 
			_ID3V2_FRAME_BODY_BODY_LENGTH);								   // 生成注释内容
	id3v2Comment.nBodyLengthInBytes = wcslen(id3v2Comment.chBody) << 1;    // 生成注释内容长度
	UINT nLengthInBytes = 10 + id3v2Comment.nBodyLengthInBytes;            // 计算帧内容总大小，编码1字节，语种3字节，注释描述2个字，内容Unicode前缀1个字，共10字节，加上内容
	return GenerateId3v2FrameHeader(id3v2FrameType_comment, 
									id3v2Comment.header, nLengthInBytes);  // 生成帧头
}
UINT Mp3Tag::GenerateId3v2PictureFrame(ID3v2PictureFrame & id3v2Picture) {
	memset(&id3v2Picture, 0, sizeof(id3v2Picture));									  // 初始化结构
	id3v2Picture.chTextEncoding = _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE;				  // 生成编码方式
	id3v2Picture.chMIMEType = _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE[
		pAudioFile->GetPicInfo().nFileType];										  // 生成MIME对应编码
	id3v2Picture.nMIMETypeLengthInBytes = _ID3V2_PICTURE_FRAME_MIME_TYPE_CODE_LENGTH[
		pAudioFile->GetPicInfo().nFileType];										  // 记录编码长度
	id3v2Picture.chPictureType = _ID3V2_PICTURE_FRAME_PICTURE_TYPE_COVER_FRONT;       // 生成图片类型描述
	memcpy(id3v2Picture.chDescrip, _ID3V2_DEFAULT_PICTURE_DESCRIP,
		   _ID3V2_PICTURE_FRAME_DESCRIP_LENGTH);									  // 生成对图片的默认描述
	MultiDelete(id3v2Picture.chPictureData); // 先释放指针（如果已有内容）
	CFile picFile;
	picFile.Open(pAudioFile->GetPicInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	id3v2Picture.nPicDataLengthInBytes = static_cast<UINT>(picFile.GetLength());  // 记录数据总长度
	id3v2Picture.chPictureData = new char[id3v2Picture.nPicDataLengthInBytes];    // 分配记录图片数据的空间，Delete要紧跟在图片数据写入文件之后
	picFile.Seek(0, CFile::begin);
	picFile.Read(id3v2Picture.chPictureData, id3v2Picture.nPicDataLengthInBytes); // 记录图片数据
	picFile.Close();	
	UINT nBodyLengthInBytes = 6 + id3v2Picture.nMIMETypeLengthInBytes 
		+ id3v2Picture.nPicDataLengthInBytes;									  // 计算帧内容总大小，编码1字节，类型描述1字节，图片默认描述2个字，共6字节，加上MIME和数据内容
	return GenerateId3v2FrameHeader(id3v2FrameType_picture, id3v2Picture.header, 
									nBodyLengthInBytes);						  // 生成帧头
}
UINT Mp3Tag::GenerateId3v2FrameBody(ID3v2FrameBody & id3v2FrameBody, const wchar_t* pContent) {
	memset(&id3v2FrameBody, 0, sizeof(id3v2FrameBody));					     // 初始化结构
	id3v2FrameBody.chEncode = _ID3V2_FRAME_BODY_ENCODE_USE_UNICODE;			 // 生成内容编码提示符
	memcpy(id3v2FrameBody.chUnicodePrefix, _ID3V2_FRAME_USE_UNICODE_PREFIX, 
		   _ID3V2_FRAME_BODY_UNICODE_PREFIX_LENGTH);						 // 生成Unicode编码字符串前缀
	wcsncpy(id3v2FrameBody.chBody, pContent, _ID3V2_FRAME_BODY_BODY_LENGTH); // 生成帧内容
	id3v2FrameBody.nBodyLengthInBytes = wcslen(id3v2FrameBody.chBody) << 1;  // 记录帧内容长度，wcslen单位是字，故乘2
	return 3 + id3v2FrameBody.nBodyLengthInBytes;							 // 返回帧内容总长度，编码标识符1字节+Unicode字符串前缀2字节+内容
}
UINT Mp3Tag::GenerateId3v2FrameHeader(const UINT & nFrameType, ID3v2FrameHeader & id3v2FrameHeader, const UINT & nBodyLengthInBytes) {
	memset(&id3v2FrameHeader, 0, sizeof(id3v2FrameHeader));						  // 初始化结构	
	memcpy(id3v2FrameHeader.chFrameID, _ID3v2_FRAME_HEADER_ID_TYPES[nFrameType], 
		   _ID3V2_FRAME_HEADER_ID_LENGTH);										  // 生成帧标识ID	
	UintTo4BytesUCharBigEndian(nBodyLengthInBytes, id3v2FrameHeader.chFrameSize); // 生成帧内容大小，UINT是以小端形式存放的，而ID3v2要求的存放方式类似“大端”，所以要将Size反转
	return nBodyLengthInBytes + _ID3V2_FRAME_HEADER_LENGTH;
}

void ApeTag::EditAPEv2Tag() {
	APEv2 apev2; // 生成APEv2标签结构
	GenerateAPEv2Tag(apev2);
	CFile file;	 // 以二进制方式打开MP3文件进行写入
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);

	// 判断APE文件是否已有APEv2标签（默认APEv2标签在文件尾部，官方不推荐将APEv2标签放在文件首部）
	APEv2TagFooter apev2TagFooterReader;
	memset(&apev2TagFooterReader, 0, sizeof(apev2TagFooterReader));
	file.Seek(-_APEV2_TAG_HEADER_FOOTER_LENGTH, CFile::end);
	file.Read(&apev2TagFooterReader, _APEV2_TAG_HEADER_FOOTER_LENGTH);
	int nHasNoTag = memcmp(apev2TagFooterReader.chIdentifier, _APEV2_TAG_IDENTIFIER, _APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH);
	if(!nHasNoTag){ // 返回0，代表字节都相同，即APE文件已含有APEv2标签
		file.SetLength(file.GetLength() - apev2TagFooterReader.nTagSize); // 将尾部的Items+标签尾都去除
		if(apev2TagFooterReader.chFlags[3] == 0x80){ // 含有标签头，去掉，判断是否有标签头 参考Flag的意义 http://wiki.hydrogenaud.io/index.php?title=Ape_Tags_Flags
			file.SetLength(file.GetLength() - _APEV2_TAG_HEADER_FOOTER_LENGTH);
		}
	}

	// 在APE文件尾部写入APEv2标签
	file.Seek(0, CFile::end); 
	WriteAPEv2(file, apev2);
	file.Close();
}
void ApeTag::GenerateAPEv2Tag(APEv2 & apev2) {
	memset(&apev2, 0, sizeof(apev2)); // 初始化结构
	
	// 生成APEv2Items结构，记录标签总大小，不含标签头，含标签尾，记录Item个数
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
	GenerateAPEv2TagHeaderAndFooter(apev2, nTagSize, nItemCount); // 生成APEv2标签头/尾结构
}
UINT ApeTag::GenerateAPEv2Item(const UINT & nItemKey, APEv2Item & apev2Item, const wchar_t* pContent) {
	memset(&apev2Item, 0, sizeof(apev2Item));						    // 初始化结构
	apev2Item.pKey = _APEV2_ITEM_KEY[nItemKey];						    // 生成ItemKey类型
	apev2Item.nKeyLengthInBytes = _APEV2_ITEM_KEY_LENGTH[nItemKey];     // 生成ItemKey大小
	UnicodeToUTF8(pContent, apev2Item.chBody, _APEV2_ITEM_BODY_LENGTH); // 生成Item内容及对应大小
	apev2Item.nBodyLengthInBytes = strlen(apev2Item.chBody);
	UINT nItemSize = sizeof(apev2Item.nBodyLengthInBytes) // 计算Item总大小，记录Item内容大小的UINT 4字节，Flags所占字节，ItemKey所占字节，Item内容所占字节
		+ _APEV2_ITEM_FLAGS_LENGTH 
		+ apev2Item.nKeyLengthInBytes 
		+ apev2Item.nBodyLengthInBytes;
	return nItemSize;
}
void ApeTag::GenerateAPEv2TagHeaderAndFooter(APEv2 & apev2, const UINT & nTagSize, const UINT & nItemCount) {
	memset(&apev2.apev2TagHeader, 0, sizeof(apev2.apev2TagHeader)); // 初始化结构
	memset(&apev2.apev2TagFooter, 0, sizeof(apev2.apev2TagFooter));
	memcpy(apev2.apev2TagHeader.chIdentifier, _APEV2_TAG_IDENTIFIER, 
			_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH);            // 生成标签标识符
	memcpy(apev2.apev2TagFooter.chIdentifier, _APEV2_TAG_IDENTIFIER,
			_APEV2_TAG_HEADER_FOOTER_IDENTIFIER_LENGTH);
	apev2.apev2TagHeader.nVersion = _APEV2_TAG_VERSION_2_0;	        // 生成版本号
	apev2.apev2TagFooter.nVersion = _APEV2_TAG_VERSION_2_0;
	apev2.apev2TagHeader.nTagSize = nTagSize;					    // 生成标签总大小（不含标签头，含标签尾）
	apev2.apev2TagFooter.nTagSize = nTagSize;
	apev2.apev2TagHeader.nItemCount = nItemCount;					// 生成ItemCount
	apev2.apev2TagFooter.nItemCount = nItemCount;
	memcpy(apev2.apev2TagHeader.chFlags, _APEV2_TAG_FLAGS_HEADER, 	// 生成头尾标识符
		   _APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH);
	memcpy(apev2.apev2TagFooter.chFlags, _APEV2_TAG_FLAGS_FOOTER, 
		   _APEV2_TAG_HEADER_FOOTER_FLAGS_LENGTH);
}

void FlacTag::EditFLACTag() {
	FLAC flac;												 // 初始化FLAC文件头结构
	memset(&flac, 0, sizeof(flac));
	GenerateFLACVorbisComment(flac.vorbisComment);			 // 生成FLAC标签块
	if(PathFileExists(pAudioFile->GetPicInfo().chFilePath)){ // 生成FLAC图片块
		GenerateFLACPicture(flac.picture);
	}			  
	CFile file;												 // 以二进制方式打开MP3文件进行写入
	file.Open(pAudioFile->GetAudioInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	file.Seek(0, CFile::begin);
	file.Read(flac.chIdentifier, _FLAC_IDENTIFIER_LENGTH);   // 读取标识符
	assert(!memcmp(flac.chIdentifier, _FLAC_IDENTIFIER, _FLAC_IDENTIFIER_LENGTH)); // 若标识符不对，则是非法FLAC文件
	FLACBlockHeader headerReader;
	while(1){ // 进入迭代读取块信息，将信息记录到区块中
		memset(&headerReader, 0, sizeof(headerReader));
		file.Read(&headerReader, _FLAC_BLOCK_HEADER_LENGTH);			   // 读取块头，判断是否是FLAC文件中最后一个块
		UINT nBlockType = headerReader.chType & _FLAC_GET_BLOCK_TYPE_MASK; // 取出块类型
		UINT nBlockSize = headerReader.chSize[0] * 0x10000			       // 计算块内容长度
			+ headerReader.chSize[1] * 0x100
			+ headerReader.chSize[2];
		if(nBlockType != flacBlockType_vorbisComment && nBlockType != flacBlockType_picture){ // 除了标签、图片信息以外，都记录原有的信息
			flac.BlockList[nBlockType].header.chType = static_cast<UCHAR>(nBlockType); // 记录块类型，此时最高位都是0，表示不是最后一个块
			memcpy(flac.BlockList[nBlockType].header.chSize, headerReader.chSize, 
				   _FLAC_BLOCK_HEADER_SIZE_LENGTH);									   // 拷贝原块内容大小
			flac.BlockList[nBlockType].body.nBodyLengthInBytes = nBlockSize;		   // 记录块内容长度
			MultiDelete(flac.BlockList[nBlockType].body.chBody);				       // 先释放指针原来指向的内容
			flac.BlockList[nBlockType].body.chBody = new char[nBlockSize];			   // Delete紧跟在写入文件语句之后
			file.Read(flac.BlockList[nBlockType].body.chBody, nBlockSize);
		}
		else{ // 不记录原有标签、图片信息，跳过
			file.Seek(nBlockSize, CFile::current);
		}
		if(headerReader.chType & _FLAC_IS_LAST_BLOCK_MASK) { // 是最后一个块，则需要退出循环
			flac.BlockList[flacBlockType_padding].header.chType = _FLAC_PADDING_AS_LAST_BLOCK; // 将Padding块置为最后一个块
			if(!flac.BlockList[flacBlockType_padding].body.nBodyLengthInBytes){ // 如果原本的FLAC文件中没有Padding，则添加
				flac.BlockList[flacBlockType_padding].body.nBodyLengthInBytes = _FLAC_DEFAULT_PADDING_LENGTH; 
				memcpy(flac.BlockList[flacBlockType_padding].header.chSize, _FLAC_DEFAULT_PADDING_SIZE, 
					   _FLAC_BLOCK_HEADER_SIZE_LENGTH);							// 生成块大小
				MultiDelete(flac.BlockList[flacBlockType_padding].body.chBody); // 释放原有内容（如果有）
				flac.BlockList[flacBlockType_padding].body.chBody = new char[_FLAC_DEFAULT_PADDING_LENGTH]; // Delete紧跟在写入文件之后
				memset(flac.BlockList[flacBlockType_padding].body.chBody, 0, _FLAC_DEFAULT_PADDING_LENGTH);
			}
			break; // 退出循环
		}
	}
	UINT nSrcSize = static_cast<UINT>(file.GetLength() - file.GetPosition()); // 计算原音频内容长度
	char* pSrcBuffer = new char[nSrcSize]; // Delete紧跟在写入文件语句之后
	file.Read(pSrcBuffer, nSrcSize);	   // 取出原音频内容
	file.SetLength(0);					   // 清空FLAC文件

	// 写入文件头，再写入原音频内容
	file.Seek(0, CFile::begin);
	WriteFLAC(file, flac);
	file.Write(pSrcBuffer, nSrcSize);
	MultiDelete(pSrcBuffer);
	file.Close();
}
void FlacTag::GenerateFLACVorbisComment(FLACVorbisComment & vorbisComment) {
	memset(&vorbisComment, 0, sizeof(vorbisComment));								   // 初始化结构
	vorbisComment.nDescripLengthInBytes = _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH; // 生成标签块描述长度
	memcpy(vorbisComment.chDescrip, _FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP, 
			_FLAC_DEFAULT_VORBIS_COMMENT_DESCRIP_LENGTH);							   // 生成标签块描述

	// 生成各个标签帧，记录帧总个数，计算块内容大小（不含块头）
	vorbisComment.nFrameCount = 0;
	UINT nLengthInBytes = 0;
	if(wcslen(pAudioFile->GetTagInfo().chTitle) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_title, vorbisComment.title, pAudioFile->GetTagInfo().chTitle); }
	if(wcslen(pAudioFile->GetTagInfo().chArtist) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_artist, vorbisComment.artist, pAudioFile->GetTagInfo().chArtist); }
	if(wcslen(pAudioFile->GetTagInfo().chAlbum) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_album, vorbisComment.album, pAudioFile->GetTagInfo().chAlbum); }
	if(wcslen(pAudioFile->GetTagInfo().chYear) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_year, vorbisComment.year, pAudioFile->GetTagInfo().chYear); }
	if(wcslen(pAudioFile->GetTagInfo().chStyle) && (++vorbisComment.nFrameCount)) { nLengthInBytes += GenerateFLACFrame(flacFrameType_style, vorbisComment.style, pAudioFile->GetTagInfo().chStyle); }
	assert(nLengthInBytes && L“Nothing to write in!”); 
	if(++vorbisComment.nFrameCount){ nLengthInBytes += GenerateFLACFrame(flacFrameType_comment, vorbisComment.comment, _FLAC_DEFAULT_COMMENT); }
	vorbisComment.header.chType = _FLAC_BLOCK_VORBIS_COMMENT_TYPE;		     // 生成块类型信息
	nLengthInBytes += _FLAC_BLOCK_VORBIS_COMMENT_BASE_LENGTH;				 // 块内容中除开帧以外的基础长度
	nLengthInBytes += vorbisComment.nFrameCount * 4;						 // 每个帧前面描述内容长度的整型4字节
	UintTo3BytesUCharBigEndian(nLengthInBytes, vorbisComment.header.chSize); // 生成块内容大小（不含块头）
}
void FlacTag::GenerateFLACPicture(FLACPicture & picture) {
	memset(&picture, 0, sizeof(picture));		    // 初始化结构
	memcpy(picture.chPicType, _FLAC_BLOCK_PICTURE_PIC_TYPE_COVER_FRONT, 
		   _FLAC_BLOCK_PICTURE_PIC_TYPE_LENGTH);    // 生成图片类型描述
	picture.nMIMETypeLengthInBytes = _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE_LENGTH[pAudioFile->GetPicInfo().nFileType]; 
    UintTo4BytesUCharBigEndian(picture.nMIMETypeLengthInBytes, picture.chMIMESize);				 // 生成MIME类型符大小，UINT是以小端形式存放的，而FLAC要求的存放方式类似“大端”，所以要将Size反转
	picture.chMIMEType = _FLAC_BLOCK_PICTURE_MIME_TYPE_CODE[pAudioFile->GetPicInfo().nFileType]; // 生成MIME类型符
	
	// 生成图片数据
	MultiDelete(picture.chPicData); // 先释放指针（如果已有内容）
	CFile picFile;
	picFile.Open(pAudioFile->GetPicInfo().chFilePath, CFile::modeReadWrite | CFile::typeBinary);
	picture.nPicDataLengthInBytes = static_cast<UINT>(picFile.GetLength());
	picture.chPicData = new char[picture.nPicDataLengthInBytes]; // Delete要紧跟在图片数据写入文件之后
	picFile.Seek(0, CFile::begin);
	picFile.Read(picture.chPicData, picture.nPicDataLengthInBytes);
	picFile.Close();
	UintTo4BytesUCharBigEndian(picture.nPicDataLengthInBytes, picture.chPicDataSize); // 生成图片数据大小，UINT是以小端形式存放的，而FLAC要求的存放方式类似“大端”，所以要将Size反转
	picture.header.chType = _FLAC_BLOCK_PICTURE_TYPE;     // 生成块类型信息
	UINT nLengthInBytes = _FLAC_BLOCK_PICTURE_BASE_LENGTH // 计算块内容大小（不含块头）
		+ picture.nMIMETypeLengthInBytes
		+ picture.nPicDataLengthInBytes;
	UintTo3BytesUCharBigEndian(nLengthInBytes, picture.header.chSize); // 生成块内容大小（不含块头）
}
UINT FlacTag::GenerateFLACFrame(const UINT & nFrameType, FLACFrame & frame, const wchar_t* pContent) {
	memset(&frame, 0, sizeof(frame)); // 初始化结构
	strncpy(frame.chBody, _FLAC_FRAME_TYPE_IDENTIFIER[nFrameType], _FLAC_FRAME_BODY_LENGTH); // 生成帧内容类型标识
	char buffer[_FLAC_FRAME_BODY_LENGTH];
	UnicodeToUTF8(pContent, buffer, _FLAC_FRAME_BODY_LENGTH); // 生成帧内容
	strncat(frame.chBody, buffer, _FLAC_FRAME_BODY_LENGTH - strlen(frame.chBody));
	frame.nBodyInBytes = strlen(frame.chBody); // 记录帧内容长度
	return frame.nBodyInBytes;
}

