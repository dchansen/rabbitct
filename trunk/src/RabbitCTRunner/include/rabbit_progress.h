#ifndef __LME_RCT_RABBIT_PROGRESS__H
#define __LME_RCT_RABBIT_PROGRESS__H 1

// Defines
// 
#define RABBIT_PROGRESS 1

using namespace std;

class RabbitProgress
{
public:
    // @param size determines which bunny to use
    RabbitProgress(const unsigned int maxValue, const unsigned int rabbitSize)
	: mRabbitSize(rabbitSize), mMax(maxValue), mNextLine(1)
    {
	if (mRabbitSize >= 1024) {
	    mRabbit = rabbit1024;
	    mRabbitLines = 19;
	} else if (mRabbitSize >= 512) {
	    mRabbit = rabbit512;
	    mRabbitLines = 16;
	} else if (mRabbitSize >= 256) {
	    mRabbit = rabbit256;
	    mRabbitLines = 9;
	} else /*if (mRabbitSize >= 128)*/ {
	    mRabbit = rabbit128;
	    mRabbitLines = 3;
	}
    }
    void progress(const unsigned int pos)
    {
	// ex.: rabbit has 3 lines. print first line after 1/3 of total progress
	if ((float)mNextLine / (float)mRabbitLines <= (float)pos / (float)mMax)
	{
	    cout << " " << mRabbit[mNextLine-1] << endl;
	    ++mNextLine;
	}
    }
private:
    unsigned int mNextLine;
    unsigned int mMax;
    unsigned int mRabbitSize;
    unsigned int mRabbitLines;
    char** mRabbit;
    static char* rabbit128 [3];
    static char* rabbit256 [9];
    static char* rabbit512 [16];
    static char* rabbit1024[19];
};
char* RabbitProgress::rabbit128 [3] = {
    " (\\_/)",
    "(='.'=)",
    "(\")_(\")"
};
char* RabbitProgress::rabbit256 [9] = {
    "   _     _     ",
    "   \\`\\ /`/   ",
    "    \\ V /     ",
    "    /. .\\     ",
    "   =\\ T /=    ",
    "    / ^ \\     ",
    "   /\\\\ //\\  ",
    " __\\ \" \" /__",
    "(____/^\\____) "
};
char* RabbitProgress::rabbit512[16] = {
    "    / \\     / \\     ",
    "   {   }   {   }      ",
    "   {   {   }   }      ",
    "    \\   \\ /   /     ",
    "     \\   Y   /       ",
    "     .-\"`\"`\"-.     ",
    "   ,`         `.      ",
    "  /             \\    ",
    " /               \\   ",
    "{     ;\"\";,       } ",
    "{  /\";`'`,;       }  ",
    " \\{  ;`,'`;.     /   ",
    "  {  }`""`  }   /}    ",
    "  {  }      {  //     ",
    "  {||}      {  /      ",
    "  `\"'       `\"'     "
};
char* RabbitProgress::rabbit1024 [19] = {
    "        /|      __    ",
    "       / |   ,-~ /    ",
    "      Y :|  //  /     ",
    "      | jj /( .^      ",
    "      >-\"~\"-v\"     ",
    "     /       Y        ",
    "    jo  o    |        ",
    "   ( ~T~     j        ",
    "    >._-' _./         ",
    "   /   \"~\"  |       ",
    "  Y     _,  |         ",
    " /| ;-\"~ _  l        ",
    "/ l/ ,-\"~    \\      ",
    "\\//\\/      .- \\    ",
    " Y        /    Y*     ",
    " l       I     !      ",
    " ]\\      _\\    /\"\\",
    "(\" ~----( ~   Y.  )  ",
    "~~~~~~~~~~~~~~~~~~~   "
};


#endif // __LME_RCT_RABBIT_PROGRESS__H
