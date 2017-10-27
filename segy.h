#include <iostream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <Python.h>
#include <tchar.h>
#include <vector>

#include <winsock2.h>
#pragma  comment(lib,"ws2_32.lib")
#include <chrono>

using namespace std;

struct binaryFileHeader{   //如果要使用的话需要变换
    int JOB_ID;                
    int LINE_NUM;
    int REEL_NUM;
    short int NUM_OF_TRACE;
    short int NUM_OF_AUX;
    short int INTERVAL_MS;
    short int INTERVAL_MS_ORI;
    unsigned short int NUM_OF_SAMPLES; // number of samples 一个trace中的样本点
    unsigned short int NUM_OF_SAMPLES_ORI;
    short int SAMPLE_FORMAT;
    short int ENSEMBLE;
    short int TRACE_SORT;
    short int VERT_SUM;
    short int SWEEP_FREQ_START;
    short int SWEEP_FREQ_END;
    short int SWEEP_LENGTH;
    short int SWEEP_TYPE;
    short int SWEEP_NUM_CHANNEL;
    short int SWEEP_TAPER_LEN_START;
    short int SWEEP_TAPER_LEN_END;
    short int TAPER_TYPE;
    short int CORRELATED;
    short int BINARY_GAIN;
    short int AMP_RECOR;
    short int MEASURE_SYSTEM;
    short int IMPULSE_POLAR;
    short int POLAR_CODE;
    char UNNASSIGNED1 [240];
    short int SEGY_REV_NUM;
    short int FIXED_LEN;
    short int NUM_EXT_HEAD;
    char UNNASSIGNED2 [94];
};


struct traceHeader {
    int TRACE_SEQ_GLOBAL;
    int TRACE_SEQ_LOCAL;
    int ORI_RECORD_NUM;
    int TRACE_NUM_FIELD;
    int SOURCE_POINT;
    int ENSEMBLE_NUM;
    int ENS_TRACE_NUM;
    short int TRACE_CODE;
    short int NUM_VERT_SUM;
    short int NUM_HORZ_SUM;
    short int DATA_USE;
    int DIST_CENT_RECV;
    int RECV_GRP_ELEV;
    int SURF_ELEV_SRC;
    int SOURCE_DEPTH;
    int DATUM_ELEV_RECV;
    int DATUM_ELAV_SRC;
    int WATER_DEPTH_SRC;
    int WATER_DEPTH_GRP;
    short int SCALE_DEPTH;
    short int SCALE_COOR;
    int SRC_COOR_X;
    int SRC_COOR_Y;
    int GRP_COOR_X;
    int GRP_COOR_Y;
    short int COOR_UNIT;
    short int WEATHER_VEL;
    short int SWEATHER_VEL;
    short int UPHOLE_T_SRC;
    short int UPHOLE_T_GRP;
    short int SRC_STA_CORRC;
    short int GRP_STA_CORRC;
    short int TOTAL_STA;
    short int LAG_TIME_A;
    short int LAG_TIME_B;
    short int DELAY_T;
    short int MUTE_T_STRT;
    short int MUTE_T_END;
    unsigned short int NUM_OF_SAMPL;
    unsigned short int SAMPLE_INTRVL;
    short int GAIN_TYPE;
    short int GAIN_CONST;
    short int GAIN_INIT;
    short int CORRLTD;
    short int SWEEP_FREQ_START;
    short int SWEEP_FREQ_END;
    short int SWEEP_LENGTH;
    short int SWEEP_TYPE;
    short int SWEEP_TAPER_LEN_START;
    short int SWEEP_TAPER_LEN_END;
    short int TAPER_TYPE;
    short int ALIAS_FREQ;
    short int ALIAS_SLOPE;
    short int NOTCH_FREQ;
    short int NOTCH_SLOPE;
    short int LOWCUT_FREQ;
    short int HIGHCUT_FREQ;
    short int LOWCUT_SLOPE;
    short int HIGHCUT_SLOPE;
    short int YEAR;
    short int DAY;
    short int HOUR;
    short int MINUTE;
    short int SECOND;
    short int TIME_CODE;
    short int WEIGHT_FACT;
    short int GEOPHNE_ROLL;
    short int GEOPHNE_TRACE;
    short int GEOPHNE_LAST;
    short int GAP_SIZE;
    short int OVER_TRAVEL;
    int ENS_COOR_X;
    int ENS_COOR_Y;
    int INLINE;
    int CROSS;
    int SHOOTPOINT;
    short int SHOOTPOINT_SCALE;
    short int TRACE_UNIT;
    char TRANSD_CONST [6];
    short int TRANSD_UNIT;
    short int TRACE_IDENT;
    short int SCALE_TIME;
    short int SRC_ORIENT;
    char SRC_DIRECTION [6];
    char SRC_MEASUREMT [6];
    short int SRC_UNIT;
    char UNNASSIGNED1 [6];
};

//用查表的方式将ebcdic编码转换为ascii码格式
static const unsigned char e2a[256] = {
          0,  1,  2,  3,156,  9,134,127,151,141,142, 11, 12, 13, 14, 15,
         16, 17, 18, 19,157,133,  8,135, 24, 25,146,143, 28, 29, 30, 31,
        128,129,130,131,132, 10, 23, 27,136,137,138,139,140,  5,  6,  7,
        144,145, 22,147,148,149,150,  4,152,153,154,155, 20, 21,158, 26,
         32,160,161,162,163,164,165,166,167,168, 91, 46, 60, 40, 43, 33,
         38,169,170,171,172,173,174,175,176,177, 93, 36, 42, 41, 59, 94,
         45, 47,178,179,180,181,182,183,184,185,124, 44, 37, 95, 62, 63,
        186,187,188,189,190,191,192,193,194, 96, 58, 35, 64, 39, 61, 34,
        195, 97, 98, 99,100,101,102,103,104,105,196,197,198,199,200,201,
        202,106,107,108,109,110,111,112,113,114,203,204,205,206,207,208,
        209,126,115,116,117,118,119,120,121,122,210,211,212,213,214,215,
        216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,
        123, 65, 66, 67, 68, 69, 70, 71, 72, 73,232,233,234,235,236,237,
        125, 74, 75, 76, 77, 78, 79, 80, 81, 82,238,239,240,241,242,243,
         92,159, 83, 84, 85, 86, 87, 88, 89, 90,244,245,246,247,248,249,
         48, 49, 50, 51, 52, 53, 54, 55, 56, 57,250,251,252,253,254,255
};

class segy
{
public:
    segy();
    ~segy();
    void OpenFile(std::string);
    void SaveFile();
    void CloseFile();
    void GetAllTrace();
    void ReadAllTrace();
    void SetTrace();
   
    void PrintTraceHeader();
    void PrintTextHeader();
    void PrintBinaryHeader();
	void PrintTraceData(int traceNum,int sampleNum,float **Trace);
	void DrawSegy(int TraceNum,int sampleNum,float **Trace);
	void getBuf(const char* const src,
		char* const &buf,
		const int &offset,
		const int &len);
	void switchByte(char* const c,const int &len);
    
    unsigned char TFileHead_ [3200];
    unsigned char ExTFileHead_ [3200];
    binaryFileHeader BFileHead_;
    traceHeader traceHeader_;
    float **Trace_;  //二维的地震道矩阵
	int fileLength;  //文件长度
	int traceNum;   // 地震道的道数   
	int sampleNum; //采样点数
    
    std::string filename_;
    std::fstream in_;
private:
    int toLitteEnd(int);
    unsigned int toLitteEnd(unsigned int);
    unsigned short int toLitteEnd(unsigned short int);
    short int toLitteEnd( short int);
    float toIeee(uint32_t ibm);
};

//Constuctor
segy::segy(){
}

//De-constuctor
segy::~segy(){
}

int segy::toLitteEnd(int a){
    unsigned short int tmp1=(a>>16);
    unsigned short int tmp2=(a* 0x0000FFFF);
    tmp2 = toLitteEnd(tmp2);
    tmp1 = toLitteEnd(tmp1);
    
    int b = (int)tmp2;
    b = b << 16;
    b = b | (int)tmp1;

    return b;
}

unsigned int segy::toLitteEnd(unsigned int a){
    unsigned short int tmp1=(a>>16);
    unsigned short int tmp2=(a* 0x0000FFFF);
    tmp2 = toLitteEnd(tmp2);
    tmp1 = toLitteEnd(tmp1);
    
    unsigned int b = (unsigned int)tmp2;
    b = b << 16;
    b = b | (unsigned int)tmp1;

    return b;
}

unsigned short int segy::toLitteEnd(unsigned short int a){
    unsigned short int tmp = a>>8;
    return (a<<8) | (tmp);
}

short int segy::toLitteEnd( short int a){
    short int tmp = a>>8;
    return (a<<8) | (tmp);
}

// Function below credits to Sebastian Good,
// http://www.palladiumconsulting.com/2014/09/little-performance-explorations-c/
float segy::toIeee(uint32_t ibm)   //ibm编码转变为ieee编码
{
    ibm = ntohl(ibm);
    uint32_t fr; /* fraction */
    int32_t exp; /* exponent */
    int32_t sgn; /* sign */
 
    /* split into sign, exponent, and fraction */
    fr = ibm;
    sgn = (int32_t)(fr >> 31); /* save sign */
    fr <<= 1; /* shift sign out */
    exp = (int32_t)(fr >> 25); /* save exponent */
    fr <<= 7; /* shift exponent out */
 
    if (fr == 0)
    { /* short-circuit for zero */
        exp = 0;
        goto done;
    }
 
    /* adjust exponent from base 16 offset 64 radix point before first digit to base 2 offset 127 radix point after first digit */
    /* (exp - 64) * 4 + 127 - 1 == exp * 4 - 256 + 126 == (exp << 2) - 130 */
    exp = (exp << 2) - 130;
 
    /* (re)normalize */
    while (fr < 0x80000000)
    { /* 3 times max for normalized input */
        --exp;
        fr <<= 1;
    }
 
    if (exp <= 0)
    { /* underflow */
        if (exp < -24) /* complete underflow - return properly signed zero */
            fr = 0;
        else /* partial underflow - return denormalized number */
            fr >>= -exp;
        exp = 0;
    }
    else if (exp >= 255)
    { /* overflow - return infinity */
        fr = 0;
        exp = 255;
    }
    else
    { /* just a plain old number - remove the assumed high bit */
        fr <<= 1;
    }
 
    done:
        /* put the pieces back together and return it */
        uint32_t ieee = ((uint32_t)(fr >> 9)) | ((uint32_t)(exp << 23)) | ((uint32_t)(sgn << 31));
        float result;
        memcpy(&result, &ieee, sizeof(float));
        return result;
}

void segy::OpenFile(std::string fl)  //打开文件  读取文件
{
    char temp [4000];  
    filename_ = fl;
	FILE *ff;
    in_.open(filename_.c_str(),std::ifstream::in); //读入文件
    if(in_.fail()) 
    {
        printf("Error opening file %s \n",filename_.c_str());
        exit(0);
    }
	ff = fopen(filename_.c_str(),"rb");
	if (ff==NULL)
	{
		printf("Error opening file %s \n",filename_.c_str());
        exit(0);
	}
	in_.seekg (0, in_.end); //将指针移动到文件末尾
	fileLength = in_.tellg();//得到文件的长度

	in_.seekg(0,in_.beg);//重新将指针移动到文件头

    memset( TFileHead_, '\0', sizeof(char) * 3200 );  //内存初始化，全初始化为0
    in_.read((char *)TFileHead_, sizeof(TFileHead_));//将前3200字节写入TFileHead中
    
    in_.read(temp, sizeof(BFileHead_));//向temp中写入二进制流即400字节的内容
    std::memcpy(&BFileHead_,temp,sizeof(BFileHead_));//将二进制流的内容拷贝到BFileHead_中去
   
	//	printf("文件的长度是：：%d",fileLength);
    /*
	 * 文件长度减去3600的道头，剩下的长度就是地震道道头与数据的长度。
	 * toLitteEnd(BFileHead_.NUM_OF_SAMPLES)是指一个地震道中的样本点数，每一个4字节
	 * 240字节指的是道头的长度。
	 */
	traceNum = (fileLength-3600)/(240+4*toLitteEnd(BFileHead_.NUM_OF_SAMPLES));//地震道道数
	sampleNum = toLitteEnd(BFileHead_.NUM_OF_SAMPLES); //采样点数
	printf("地震道（trace）的道数是：%d道\n",traceNum);
	printf("每个地震道中采样点数是：%d个\n",toLitteEnd(BFileHead_.NUM_OF_SAMPLES));
	Trace_ = (float**)malloc(sizeof(float)*traceNum);  //申请了traceNum个地震道空间
	for (int i = 0; i < traceNum; i++)
	{
		//为每一个地震道申请 BFileHead_.NUM_OF_SAMPLES 个采样点的空间
		Trace_[i] = (float*)malloc(sizeof(float)*toLitteEnd(BFileHead_.NUM_OF_SAMPLES)); 
	}
	//读取地震道数据头数据（第一个地震道的道头信息）
	in_.read(temp, sizeof(traceHeader_));
	std::memcpy(&traceHeader_,temp,sizeof(traceHeader_));//将地震道头数据240字节写到traceHeader中去
	int iTrace = 0;
	fseek(ff,3600,SEEK_SET);  //从文件起始位置开始
	while(iTrace<traceNum)
	{
		if (toLitteEnd(BFileHead_.SAMPLE_FORMAT) == 5)  //PC IEEE 类型
		{
			fseek(ff,240,SEEK_CUR); //从当前位置往后读写
//****		fread(Trace_[iTrace],sizeof(float)*toLitteEnd(BFileHead_.NUM_OF_SAMPLES),1,ff);
			char *buf(nullptr);
			buf = new char[sampleNum*sizeof(float)]; // 申请保存一道的数据空间
			fread(buf,sampleNum*sizeof(float),1,ff); // 将数据读到buf中
			int index;
			char _char2int[4];
			for (index = 0;index<sampleNum;index++)
			{
				getBuf(buf, _char2int, index*4, 4);
				memcpy(&Trace_[iTrace][index], _char2int, 4);
			}
			delete[]buf;
		}
		else if(toLitteEnd(BFileHead_.SAMPLE_FORMAT) == 1) // IBM IEEE类型 需要转换为PC IEEE
		{
			fseek(ff,240,SEEK_CUR); //从当前位置往后读写
			 //地震道数据的读取：
			char *buf(nullptr);
			buf = new char[sampleNum*sizeof(float)]; // 申请保存一道的数据空间
			fread(buf,sampleNum*sizeof(float),1,ff); // 将数据读到buf中
			int index;uint32_t tmp;
			char _char2int[4];
			for (index = 0;index<sampleNum;index++)
			{
				getBuf(buf, _char2int, index*4, 4);
				memcpy(&tmp, _char2int, 4);
				Trace_[iTrace][index] = toIeee(tmp);
			//  memcpy(&Trace_[iTrace][index], _char2int, 4);
			}
			delete[]buf;
	/*		uint8_t* buffer = new uint8_t[4*toLitteEnd(BFileHead_.NUM_OF_SAMPLES)];
			//一次读入一个地震道数据
			in_.read(reinterpret_cast<char*>(buffer), sizeof(float)*toLitteEnd(BFileHead_.NUM_OF_SAMPLES));
			for (size_t n = 0; n < toLitteEnd(BFileHead_.NUM_OF_SAMPLES); n++)  //将地震道数据放回道trace_中去
			{
				uint32_t ibmSample = *(reinterpret_cast<uint32_t*>(buffer + n*4));
				auto data = toLitteEnd(ibmSample);
				Trace_[iTrace][n] = toIeee(data);
		//		Trace_[iTrace][n] = toIeee(ibmSample);  //将地震道的数据转化为Ieee后存入Trace_中
			}
			delete[] buffer;
			in_.seekg(240,in_.cur);*/
		}
		iTrace++;  // 控制道数
	}
	PrintTraceData(20,20,Trace_);  //三个参数：道数，样本数，地震道数组 
//	PrintTraceData(traceNum,toLitteEnd(BFileHead_.NUM_OF_SAMPLES),Trace_);  //三个参数：道数，样本数，地震道数组 
	DrawSegy(traceNum,toLitteEnd(BFileHead_.NUM_OF_SAMPLES),Trace_);
}

void segy::PrintTraceHeader(){
    printf("TRACE_SEQ_GLOBAL    : %i \n",toLitteEnd( traceHeader_.TRACE_SEQ_GLOBAL) );
    printf("TRACE_SEQ_LOCAL     : %i \n", toLitteEnd( traceHeader_.TRACE_SEQ_LOCAL) );
    printf("ORI_RECORD_NUM      : %i \n",  toLitteEnd( traceHeader_.ORI_RECORD_NUM) );
    printf("TRACE_NUM_FIELD     : %i \n", toLitteEnd( traceHeader_.TRACE_NUM_FIELD) );
    printf("SOURCE_POINT        : %i \n",    toLitteEnd( traceHeader_.SOURCE_POINT) );
    printf("ENSEMBLE_NUM        : %i \n",    toLitteEnd( traceHeader_.ENSEMBLE_NUM) );
    printf("ENS_TRACE_NUM       : %i \n",   toLitteEnd( traceHeader_.ENS_TRACE_NUM) );
    printf("TRACE_CODE          : %hi \n",     toLitteEnd( traceHeader_.TRACE_CODE) );
    printf("NUM_VERT_SUM        : %hi \n",   toLitteEnd( traceHeader_.NUM_VERT_SUM) );
    printf("NUM_HORZ_SUM        : %hi \n",   toLitteEnd( traceHeader_.NUM_HORZ_SUM) );
    printf("DATA_USE            : %hi \n",       toLitteEnd( traceHeader_.DATA_USE) );
    printf("DIST_CENT_RECV      : %i \n",  toLitteEnd( traceHeader_.DIST_CENT_RECV) );
    printf("RECV_GRP_ELEV       : %i \n",   toLitteEnd( traceHeader_.RECV_GRP_ELEV) );
    printf("SURF_ELEV_SRC       : %i \n",   toLitteEnd( traceHeader_.SURF_ELEV_SRC) );
    printf("SOURCE_DEPTH        : %i \n",    toLitteEnd( traceHeader_.SOURCE_DEPTH) );
    printf("DATUM_ELEV_RECV     : %i \n", toLitteEnd( traceHeader_.DATUM_ELEV_RECV) );
    printf("DATUM_ELAV_SRC      : %i \n",  toLitteEnd( traceHeader_.DATUM_ELAV_SRC) );
    printf("WATER_DEPTH_SRC     : %i \n", toLitteEnd( traceHeader_.WATER_DEPTH_SRC) );
    printf("WATER_DEPTH_GRP     : %i \n", toLitteEnd( traceHeader_.WATER_DEPTH_GRP) );
    printf("SCALE_DEPTH         : %hi \n",    toLitteEnd( traceHeader_.SCALE_DEPTH) );
    printf("SCALE_COOR          : %hi \n",     toLitteEnd( traceHeader_.SCALE_COOR) );
    printf("SRC_COOR_X          : %i \n",      toLitteEnd( traceHeader_.SRC_COOR_X) );
    printf("SRC_COOR_Y          : %i \n",      toLitteEnd( traceHeader_.SRC_COOR_Y) );
    printf("GRP_COOR_X          : %i \n",      toLitteEnd( traceHeader_.GRP_COOR_X) );
    printf("GRP_COOR_Y          : %i \n",      toLitteEnd( traceHeader_.GRP_COOR_Y) );
    printf("COOR_UNIT           : %hi \n",      toLitteEnd( traceHeader_.COOR_UNIT) );
    printf("WEATHER_VEL         : %hi \n",    toLitteEnd( traceHeader_.WEATHER_VEL) );
    printf("SWEATHER_VEL        : %hi \n",   toLitteEnd( traceHeader_.SWEATHER_VEL) );
    printf("UPHOLE_T_SRC        : %hi \n",   toLitteEnd( traceHeader_.UPHOLE_T_SRC) );
    printf("UPHOLE_T_GRP        : %hi \n",   toLitteEnd( traceHeader_.UPHOLE_T_GRP) );
    printf("SRC_STA_CORRC       : %hi \n",  toLitteEnd( traceHeader_.SRC_STA_CORRC) );
    printf("GRP_STA_CORRC       : %hi \n",  toLitteEnd( traceHeader_.GRP_STA_CORRC) );
    printf("TOTAL_STA           : %hi \n",      toLitteEnd( traceHeader_.TOTAL_STA) );
    printf("LAG_TIME_A          : %hi \n",     toLitteEnd( traceHeader_.LAG_TIME_A) );
    printf("LAG_TIME_B          : %hi \n",     toLitteEnd( traceHeader_.LAG_TIME_B) );
    printf("DELAY_T             : %hi \n",        toLitteEnd( traceHeader_.DELAY_T) );
    printf("MUTE_T_STRT         : %hi \n",    toLitteEnd( traceHeader_.MUTE_T_STRT) );
    printf("MUTE_T_END          : %hi \n",     toLitteEnd( traceHeader_.MUTE_T_END) );
    printf("NUM_OF_SAMPL        : %hi \n",   toLitteEnd( traceHeader_.NUM_OF_SAMPL) );
    printf("SAMPLE_INTRVL       : %hi \n", toLitteEnd( traceHeader_.SAMPLE_INTRVL) );
    printf("GAIN_TYPE           : %hi \n", toLitteEnd( traceHeader_.GAIN_TYPE) );
    printf("GAIN_CONST          : %hi \n", toLitteEnd( traceHeader_.GAIN_CONST) );
    printf("GAIN_INIT           : %hi \n", toLitteEnd( traceHeader_.GAIN_INIT) );
    printf("CORRLTD             : %hi \n", toLitteEnd( traceHeader_.CORRLTD) );
    printf("SWEEP_FREQ_START    : %hi \n", toLitteEnd( traceHeader_.SWEEP_FREQ_START) );
    printf("SWEEP_FREQ_END      : %hi \n", toLitteEnd( traceHeader_.SWEEP_FREQ_END) );
    printf("SWEEP_LENGTH        : %hi \n", toLitteEnd( traceHeader_.SWEEP_LENGTH) );
    printf("SWEEP_TYPE          : %hi \n", toLitteEnd( traceHeader_.SWEEP_TYPE) );
    printf("SWEEP_TAPER_LEN_START   : %hi \n", toLitteEnd( traceHeader_.SWEEP_TAPER_LEN_START) );
    printf("SWEEP_TAPER_LEN_END : %hi \n", toLitteEnd( traceHeader_.SWEEP_TAPER_LEN_END) );
    printf("TAPER_TYPE          : %hi \n", toLitteEnd( traceHeader_.TAPER_TYPE) );
    printf("ALIAS_FREQ          : %hi \n", toLitteEnd( traceHeader_.ALIAS_FREQ) );
    printf("ALIAS_SLOPE         : %hi \n", toLitteEnd( traceHeader_.ALIAS_SLOPE) );
    printf("NOTCH_FREQ          : %hi \n", toLitteEnd( traceHeader_.NOTCH_FREQ) );
    printf("NOTCH_SLOPE         : %hi \n", toLitteEnd( traceHeader_.NOTCH_SLOPE) );
    printf("LOWCUT_FREQ         : %hi \n", toLitteEnd( traceHeader_.LOWCUT_FREQ) );
    printf("HIGHCUT_FREQ        : %hi \n", toLitteEnd( traceHeader_.HIGHCUT_FREQ) );
    printf("LOWCUT_SLOPE        : %hi \n", toLitteEnd( traceHeader_.LOWCUT_SLOPE) );
    printf("HIGHCUT_SLOPE       : %hi \n", toLitteEnd( traceHeader_.HIGHCUT_SLOPE) );
    printf("YEAR                : %hi \n", toLitteEnd( traceHeader_.YEAR) );
    printf("DAY                 : %hi \n", toLitteEnd( traceHeader_.DAY) );
    printf("HOUR                : %hi \n", toLitteEnd( traceHeader_.HOUR) );
    printf("MINUTE              : %hi \n", toLitteEnd( traceHeader_.MINUTE) );
    printf("SECOND              : %hi \n", toLitteEnd( traceHeader_.SECOND) );
    printf("TIME_CODE           : %hi \n", toLitteEnd( traceHeader_.TIME_CODE) );
    printf("WEIGHT_FACT         : %hi \n", toLitteEnd( traceHeader_.WEIGHT_FACT) );
    printf("GEOPHNE_ROLL        : %hi \n", toLitteEnd( traceHeader_.GEOPHNE_ROLL) );
    printf("GEOPHNE_TRACE       : %hi \n", toLitteEnd( traceHeader_.GEOPHNE_TRACE) );
    printf("GEOPHNE_LAST        : %hi \n", toLitteEnd( traceHeader_.GEOPHNE_LAST) );
    printf("GAP_SIZE            : %hi \n", toLitteEnd( traceHeader_.GAP_SIZE) );
    printf("OVER_TRAVEL         : %hi \n", toLitteEnd( traceHeader_.OVER_TRAVEL) );
    printf("ENS_COOR_X          : %i \n",  toLitteEnd( traceHeader_.ENS_COOR_X) );
    printf("ENS_COOR_Y          : %i \n",  toLitteEnd( traceHeader_.ENS_COOR_Y) );
    printf("INLINE              : %i \n",  toLitteEnd( traceHeader_.INLINE) );
    printf("CROSS               : %i \n",  toLitteEnd( traceHeader_.CROSS) );
    printf("SHOOTPOINT          : %i \n",  toLitteEnd( traceHeader_.SHOOTPOINT) );
    printf("SHOOTPOINT_SCALE    : %hi \n", toLitteEnd( traceHeader_.SHOOTPOINT_SCALE) );
    printf("TRACE_UNIT          : %hi \n", toLitteEnd( traceHeader_.TRACE_UNIT) );
    printf("TRANSD_UNIT         : %hi \n", toLitteEnd( traceHeader_.TRANSD_UNIT) );
    printf("TRACE_IDENT         : %hi \n", toLitteEnd( traceHeader_.TRACE_IDENT) );
    printf("SCALE_TIME          : %hi \n", toLitteEnd( traceHeader_.SCALE_TIME) );
    printf("SRC_ORIENT          : %hi \n", toLitteEnd( traceHeader_.SRC_ORIENT) );
    printf("SRC_UNIT            : %hi \n", toLitteEnd( traceHeader_.SRC_UNIT) );
    // printf("%hi \n",sgy.toLitteEnd( sgy.traceHeader_.char UNNASSIGNED1 [6]) );
    // printf("%hi \n",sgy.toLitteEnd( sgy.traceHeader_.char TRANSD_CONST [6]) );
    // printf("%hi \n",sgy.toLitteEnd( sgy.traceHeader_.char SRC_DIRECTION [6]) );
    // printf("%hi \n",sgy.toLitteEnd( sgy.traceHeader_.char SRC_MEASUREMT [6]) );
}

void segy::PrintBinaryHeader(){
    printf("JOB_ID               :%i \n",  toLitteEnd(BFileHead_.JOB_ID     )         );
    printf("LINE_NUM             :%i \n",  toLitteEnd(BFileHead_.LINE_NUM   ) );
    printf("REEL_NUM             :%i \n",  toLitteEnd(BFileHead_.REEL_NUM   ) );
    printf("NUM_OF_TRACE         :%hi \n", toLitteEnd(BFileHead_.NUM_OF_TRACE )   );
    printf("NUM_OF_AUX           :%hi \n", toLitteEnd(BFileHead_.NUM_OF_AUX ) );
    printf("INTERVAL_M           :%hi \n", toLitteEnd(BFileHead_.INTERVAL_MS)     );
    printf("INTERVAL_MS_ORI      :%hi \n", toLitteEnd(BFileHead_.INTERVAL_MS_ORI )    );
    printf("NUM_OF_SAMPLES_ORI   :%hi \n", toLitteEnd(BFileHead_.NUM_OF_SAMPLES_ORI ) );
    printf("SAMPLE_FORMAT        :%hi \n", toLitteEnd(BFileHead_.SAMPLE_FORMAT )  );
    printf("ENSEMBLE             :%hi \n", toLitteEnd(BFileHead_.ENSEMBLE   ) );
    printf("TRACE_SORT           :%hi \n", toLitteEnd(BFileHead_.TRACE_SORT ) );
    printf("VERT_SUM             :%hi \n", toLitteEnd(BFileHead_.VERT_SUM   ) );
    printf("SWEEP_FREQ_START     :%hi \n", toLitteEnd(BFileHead_.SWEEP_FREQ_START  )  );
    printf("SWEEP_FREQ_END       :%hi \n", toLitteEnd(BFileHead_.SWEEP_FREQ_END ) );
    printf("SWEEP_LENGTH         :%hi \n", toLitteEnd(BFileHead_.SWEEP_LENGTH   ) );
    printf("SWEEP_TYPE           :%hi \n", toLitteEnd(BFileHead_.SWEEP_TYPE ) );
    printf("SWEEP_NUM_CHANNEL    :%hi \n", toLitteEnd(BFileHead_.SWEEP_NUM_CHANNEL )  );
    printf("SWEEP_TAPER_LEN_START:%hi \n", toLitteEnd(BFileHead_.SWEEP_TAPER_LEN_START )  );
    printf("SWEEP_TAPER_LEN_END  :%hi \n", toLitteEnd(BFileHead_.SWEEP_TAPER_LEN_END   )  );
    printf("TAPER_TYPE           :%hi \n", toLitteEnd(BFileHead_.TAPER_TYPE ) );
    printf("CORRELATED           :%hi \n", toLitteEnd(BFileHead_.CORRELATED ) );
    printf("BINARY_GAIN          :%hi \n", toLitteEnd(BFileHead_.BINARY_GAIN )    );
    printf("AMP_RECOR            :%hi \n", toLitteEnd(BFileHead_.AMP_RECOR  ) );
    printf("MEASURE_SYSTEM       :%hi \n", toLitteEnd(BFileHead_.MEASURE_SYSTEM ) );
    printf("IMPULSE_POLAR        :%hi \n", toLitteEnd(BFileHead_.IMPULSE_POLAR  ) );
    printf("POLAR_CODE           :%hi \n", toLitteEnd(BFileHead_.POLAR_CODE ) );
    printf("SEGY_REV_NUM         :%hi \n", toLitteEnd(BFileHead_.SEGY_REV_NUM  )  );
    printf("FIXED_LEN            :%hi \n", toLitteEnd(BFileHead_.FIXED_LEN  ) );
    printf("NUM_EXT_HEAD         :%hi \n", toLitteEnd(BFileHead_.NUM_EXT_HEAD  )  );
}

void segy::PrintTraceData(int traceNum,int sampleNum,float **Trace)
{
	std::cout<<"\n输出地震道的数据：\n";
	for (int i = 0; i < traceNum; i++)
	{
		for (int j = 0; j < sampleNum; j++)
		{
			std::cout<<Trace[i][j]<<"	";
		}
		std::cout<<std::endl;
	}
}

void segy::PrintTextHeader(){
    for (size_t i = 0; i < 3200; i++)
	{
        if((i%80) == 0)   //80字节一组
            std::cout << std::endl;
            
	    std::cout << e2a[(int) (TFileHead_[i])];//读表法

	} 
    std::cout << std::endl;
}


void segy::DrawSegy(int TraceNum,int sampleNum,float **Trace)
{
	Py_Initialize();
	PyObject* pModule = NULL;
	PyObject* pFunc = NULL;
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
//	PyRun_SimpleString("sys.path.append('C:\\Users\\ZHOU\\Desktop\\C++ primer\\ConsoleApplication3\\ConsoleApplication3')");
	//PyRun_SimpleString("sys.path");
//	pModule = PyImport_ImportModule("Module_add");  //拿到模块
	pModule = PyImport_ImportModule("LoadPic");
	if (!pModule)
	{
		cout<<"open fail";
		return;
	}
	pFunc = PyObject_GetAttrString(pModule,"listLoop");
	Py_ssize_t len = TraceNum;
	Py_ssize_t wide = sampleNum;
	Py_ssize_t i = 0;
	PyObject *result = PyTuple_New(len+1);
	PyObject* leng = PyTuple_New(2);
	PyTuple_SetItem(leng,0,Py_BuildValue("i",len));
	PyTuple_SetItem(leng,1,Py_BuildValue("i",wide));
	PyTuple_SetItem(result, 0, leng);
	for (i = 0; i < len; i++) {
		PyObject *item = PyTuple_New(wide);
		for (Py_ssize_t j = 0; j < wide; j++)
		{
			PyTuple_SetItem(item, j, Py_BuildValue("d",Trace[i][j]));
		}
		PyTuple_SetItem(result, i+1, item);
	}
	PyEval_CallObject(pFunc,result);
	Py_Finalize();
}

void segy::getBuf(const char* const src,
					 char* const &buf,
					 const int &offset,
					 const int &len)
{
	int index;
	for (index = 0; index < len; index++)
		buf[index] = src[offset + index];
		switchByte(buf, len);
	return;
}

void segy::switchByte(char* const c,const int &len)  //工作站的大端模式变成PC小端模式
{
	int index;char tmp;
	for (index = 0; index < len / 2; index++)
	{
		tmp = c[index];
		c[index] = c[len - index - 1];
		c[len - index - 1] = tmp;
	}
}