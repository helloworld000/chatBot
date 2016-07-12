# chatBot
A chat bot with AI and additional features.( c++)

#ifndef __TechTron_H__
#define __TechTron_H__

//BEGIN STRINGS.H
#ifndef __STRING_H__
#define __STRING_H__

#pragma warning(disable: 4786)
#pragma warning(disable: 4503)
#include <string>
#include <vector>
#include <cstdlib>

const int INPUTMAXN = 5;
const int OUTPUTMAXN = 7;

typedef std::vector<std::string> Vstr;
std::string user_name;


bool isPunc( char c );
void stringToClear( std::string &str );
void ToUpperCase( std::string &str );
void LeftClear(std::string &str, std::string delim);
void RightClear( std::string &str, std::string delim );
void trimLR(std::string &str, std::string characters);
void copy( char *array[], Vstr &v, size_t array_size );
size_t replace( std::string &str, std::string substr1, std::string substr2 );
void SplitToWords(const std::string str, Vstr &v);

template<typename T>
void shuffle( T &array, size_t size ) 
{
    for( int i = 0; i < size; ++i ) 
    {
        int index = rand() % size;
        std::swap(array[i], array[index]);
    }
}

#endif
//END STRINGS.H

//BEGIN STRINGS.CPP
//#include "strings.h"

const std::string delim = "?!;,";

// convert a string to ToUpperCase
void ToUpperCase( std::string &str ) 
{
    int len = str.length();

    for( int i = 0; i < len; i++ ) 
    {
        if ( str[i] >= 'a' && str[i] <= 'z' ) 
        {
            str[i] -= 'a' - 'A';
        }
    }
}

// verifies that a given 
// character is a puntuation
bool isPunc(char c) 
{
    return delim.find(c) != std::string::npos;
}

// removes punctuation and redundant
// spaces from the user's input
void stringToClear( std::string &str ) 
{
    int len = str.length();
    std::string temp = "";

    char prevChar = 0;

    for(int i = 0; i < len; ++i) 
    {
        if( (str[i] == ' ' && prevChar != ' ') || !isPunc(str[i]) ) 
        {
            temp += str[i];
            prevChar = str[i];
        }   
        else if(prevChar != ' ' && isPunc(str[i]))
        {
            temp += ' ';
            prevChar = ' ';
        }
    }
    str = temp;
}

// copy the content of a string array to a vector
void copy(char *array[], Vstr &v, size_t array_size) 
{
    for(int i = 0; i < array_size; ++i) 
    {
        if(array[i] != NULL) 
        {
            v.push_back(array[i]);
        } 
        else 
        {
            break;
        }
    }
}


//replaces occurrence of substr1 in str with substr2
size_t replace(std::string &str, std::string substr1, std::string substr2) {
    size_t pos = str.find(substr1);
    if(pos != std::string::npos) {
        str.erase(pos, substr1.length());
        str.insert(pos, substr2);
    }
    return pos;
}

//removes delimeter from the end
void RightClear(std::string &str, std::string delim) {
    size_t pos = str.find_last_not_of(delim);
    if(pos != std::string::npos) {
        str.erase(pos + 1, str.length());
    }
}

//removes delimeter from the front
void LeftClear(std::string &str, std::string delim) {
    size_t pos = str.find_first_not_of(delim);
    if(pos != std::string::npos) {
        str.erase(0, pos);
    }
}

// trim all left and right characters that is specified by
// the string "characters"
void trimLR(std::string &str, std::string characters) {
    LeftClear(str, characters);
    RightClear(str, characters);
}


//splits a line into words
void SplitToWords(const std::string str, Vstr &v) {
    std::string buffer;
    for(int i = 0; i < str.length(); ++i) {
        if(!isPunc(str[i]) && !isspace(str[i]) && str[i] != '.') {
            buffer += str[i];
        } else if(!buffer.empty()) {
            v.push_back(buffer);
            buffer.erase();
        }
    }
    if((v.empty() && !buffer.empty()) || !buffer.empty()) {
        v.push_back(buffer);
    }
}

//END STRINGS.CPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <ctime>
//#include <sapi.h>
#include <map>

class TBResponse;


//mapString is used to store knowledge base
typedef std::map<std::string, std::vector<TBResponse> > mapString;  //stores (context and response string vectors) as value for string keys


typedef struct {
    char *first;
    char *second;
} personTenseChange;



class TechBot {
    
public:
    TechBot (std::string str) 
        :TB_BotName(str), TB_bQuitProgram(0), TB_Input("null")//, TB_bTTSEngineInitialised(0), TB_pVoice(NULL)
    { 
        initializeRandom();
        //Initialize_TTS_Engine();
        LoadDBclass();
    }
    ~TechBot () {
        //Release_TTS_Engine();
    };

    void readInput();
    void welcome_user();
    void answer();

    void speak(const std::string text);

    void welcome();

    
    bool quit() const {
        return TB_bQuitProgram;
    }

    void selectMatch();
    void manageRepeat();
    void manageUserRepeat();
    void eventManager(std::string str);

    void initializeRandom()
    {
        srand((unsigned) time(NULL));
    }

    void chooseOutput();

    void SavePrevinput() {
        TB_PrevInput = TB_Input;
    }

    void SavePrevresponse() {
        TB_PrevResponse = TB_Response;
    }

    void SavePrevevent() {
        TB_PrevEvent = TB_Event;
    }

    void setEvent(std::string str) {
        TB_Event = str;
    }

    void Saveinput() {
        TB_InputBackup = TB_Input;
    }

    void SetInput(std::string str) {
        TB_Input = str;
    }
    
    void restore_input() {
        TB_Input = TB_InputBackup;
    }
    
    void print_response() const {
        if(TB_Response.length() > 0) {
            std::cout << TB_Response << std::endl;
        }
        
    }
    
    void initPre_input();

    void initPre_response();

    void RecogniseTopic();

    bool isAIRepeat();

    bool isUserRepeat() const;

    bool isAIunderstand() const {
        return RespLISTonse.size() > 0;
    }

    bool null_input() const {
        return (TB_Input.length() == 0 && TB_PrevInput.length() != 0);
    }

    bool null_input_repetition() const {
        return (TB_Input.length() == 0 && TB_PrevInput.length() == 0);
    }

    bool user_want_to_quit() const {
        if(!(TB_Input.find("BYE") != std::string::npos))
            return false;
        return true;
        
        
    }

    bool same_event() const {
        return (TB_Event.length() > 0 && TB_Event == TB_PrevEvent);
    }

    bool no_response() const {
        return RespLISTonse.size() == 0;
    }

    bool isSameInput() const {
        return (TB_Input.length() > 0  && TB_Input == TB_PrevInput);
    }

    std::string generateSubP(Vstr wordList, size_t start, size_t end);
    std::string findBestKey(Vstr v);

    void initPreKeyWord(std::string &str, size_t start_pos, size_t end_pos, size_t iSize);

    bool isSimilarInput() const;

    void changePT(std::string &str);

    //void Initialize_TTS_Engine();
    //void Release_TTS_Engine();

    void addXmlTag(std::string &str);

    void LoadDBclass();
    void RespListgetter(std::vector<TBResponse> objList);

    void SaveAI_response() {
        if(!TB_Response.empty()) vResponseLog.push(TB_Response);
    }

    int findRespPos(std::string str);

   // void Saveunknown_input();

   /* void update_unkown_input_list() {
        ListOfUnknownInput.push_back(TB_Input);
    }
    */

    void Savelog();
    void Savelog(std::string str);
    bool isaskedusername();

private:
    std::string         TB_BotName;
    std::string         TB_UserName;
    std::string         TB_Input;
    std::string         TB_Response;
    std::string         TB_PrevInput;
    std::string         TB_PrevResponse;
    std::string         TB_Event;
    std::string         TB_PrevEvent;
    std::string         TB_InputBackup;
    std::string         TB_Subject;
    std::string         TB_KeyWord;
    std::string         TB_Context;
    std::string         TB_PrevContext;
    bool                TB_bQuitProgram;
    bool                TB_bTTSEngineInitialised;
   // ISpVoice            *TB_pVoice;

    Vstr                     RespLISTonse;
    //Vstr                     ListOfUnknownInput;
    std::stack<std::string>     vResponseLog;

    mapString           KnowledgeBase;

    std::fstream        logfile;
    
};


class TBResponse {
public:
    TBResponse() {}
    ~TBResponse() {}

    void addContext(std::string str) 
    {
        contextLIST.push_back(str);
    }

    void addResp(std::string str)
    {
        RespLIST.push_back(str);
    }

    Vstr getContextList() const 
    {
        return contextLIST;
    }

    Vstr getRespList() const 
    {
        return RespLIST;
    }

    void clear()
    {
        RespLIST.clear();
        contextLIST.clear();
    }

private:
    Vstr contextLIST;
    Vstr RespLIST;
};

#endif

//ENDTechTron.H


personTenseChange transposList[] = {
    {" MYSELF ", " YOURSELF "},
    {" DREAMS ", " DREAM "},
    {" WEREN'T ", " WASN'T "},  
    {" AREN'T ", " AM NOT "},
    {" I'VE ", " YOU'VE "},
    {" MINE ", " YOURS "},
    {" MY ", " YOUR "},
    {" WERE ", " WAS "},
    {" MOM ", " MOTHER "},
    {" I AM ", " YOU ARE "},
    {" I'M ", " YOU'RE "},
    {" DAD ", " FATHER "},
    {" MY ", " YOUR "},
    {" AM ", " ARE "},
    {" I'D ", " YOU'D "},
    {" I ", " YOU "},
    {" ME ", " YOU "},
    {" ARE YOU ", " I AM "},
    {" YOU "," ME "}
};

size_t transposListSize = sizeof(transposList) / sizeof(transposList[0]);

/*
void TechBot::Saveunknown_input()
{
    std::fstream fout("unknown.txt", std::ios::out);
    if(fout.fail())
    {
        throw std::string("Unable to save Unknown Input List");
    }
    Vstr::const_iterator iter = ListOfUnknownInput.begin();
    for( ; iter != ListOfUnknownInput.end(); ++iter )
    {
        fout << *iter << std::endl;
    }
    fout.flush();
    fout.close();
}
*/
void TechBot::Savelog()
{
    time_t ltime; 
    time(&ltime);
    logfile.open("log.txt", std::ios::out | std::ios::app);
    if(logfile.fail()) 
    {
        throw std::string("can't save conversation log");
    }
    logfile << "\n\nConversation log - " << ctime(&ltime) << std::endl;
}

void TechBot::Savelog(std::string str)
{
    if(str == "TechTron") 
    {
        logfile << TB_Response << std::endl;
    } 
    else if(str == "USER") 
    {
        logfile << ">" << TB_Input << std::endl;
    }
}

void TechBot::LoadDBclass()
{
    std::fstream fin("DBclass.txt", std::ios::in);
    if(fin.fail())
    {
        throw std::string("Unable to load the database: DBclass.txt");
    }

    TBResponse respObj;
    std::vector<TBResponse> RespLISTObj;
    
    std::string buffer;
    Vstr keyList;

    while(std::getline(fin, buffer))
    {
        char cSymbol = buffer[0];
        buffer.erase(0, 1);
        switch(cSymbol)
        {
        case 'K':
            keyList.push_back(buffer);
            break;
        case 'C':
            respObj.addContext(buffer);
            break;
        case 'R':
            respObj.addResp(buffer);
            break;
        case '%':
            RespLISTObj.push_back(respObj);
            respObj.clear();
            break;
        case '#':
            {
                if(RespLISTObj.empty())
                {
                    RespLISTObj.push_back(respObj);
                }

                Vstr::const_iterator iter = keyList.begin();
                for( ; iter != keyList.end(); ++iter )
                {
                    std::string temp=*iter;
                    //useinlinux//temp=temp.substr(0,temp.length()-1);
                    KnowledgeBase[temp] = RespLISTObj;
                }
                keyList.clear();
                RespLISTObj.clear();
                respObj.clear();
            }
            break;
        }
    }
    fin.close();
    
}

void TechBot::welcome()
{
    eventManager("welcome**");
    chooseOutput();
    Savelog();
    Savelog("TechTron");
    //speak(TB_Response);
    print_response();
}

void TechBot::readInput() 
{
    std::cout << ">";

    // saves the previous input
    SavePrevinput();
    std::getline(std::cin, TB_Input);
}

void TechBot::initPre_input() 
{
    stringToClear(TB_Input);
    RightClear(TB_Input, ". ");
    ToUpperCase(TB_Input);
}

void TechBot::initPre_response()
{
    if(TB_Response.find("*") != std::string::npos)
    {
        // extracting from input
        RecogniseTopic(); 
        // conjugating subject
        changePT(TB_Subject); 

        replace(TB_Response, "*", " " + TB_Subject);
    }
}

void TechBot::RespListgetter(std::vector<TBResponse> objList)
{
    std::vector<TBResponse>::const_iterator iter = objList.begin();
    for( ; iter != objList.end(); ++iter )
    {
        Vstr contextLIST = iter->getContextList();
        if(contextLIST.empty())
        {
            RespLISTonse = iter->getRespList();
        }
        else if(std::find(contextLIST.begin(), 
            contextLIST.end(), TB_PrevResponse) != contextLIST.end())
        {
            RespLISTonse = iter->getRespList();
            break;
        }
    }
}

void TechBot::RecogniseTopic()
{
    TB_Subject.erase(); // resets subject variable
    RightClear(TB_Input, " ");
    trimLR(TB_KeyWord, "_");
    size_t pos = TB_Input.find(TB_KeyWord);
    if(pos != std::string::npos)
    {
        TB_Subject = TB_Input.substr(pos + TB_KeyWord.length() + 1, TB_Input.length() - 1);     
    }
}

bool TechBot::isAIRepeat()
{
    int pos = findRespPos(TB_Response);
    if(pos > 0)
    {
        return pos + 1 < RespLISTonse.size();
    }
    return 0;
}

int TechBot::findRespPos(std::string str)
{
    int pos = -1;
    std::stack<std::string> s = vResponseLog;
    while(s.empty())   //check !s.empty()
    {
        ++pos;
        if(s.top() == str)
        {
            break;
        }
        s.pop();
    }
    return pos;
}

bool TechBot::isUserRepeat() const 
{
    std::string curinput=TB_Input;
    std::string previnput=TB_PrevInput;
    ToUpperCase(curinput);
    ToUpperCase(previnput);
    if( (previnput.length() > 0 &&
        ((curinput == previnput) || (curinput.find(previnput) != std::string::npos) ||
        (previnput.find(curinput) != std::string::npos))))
         return 1;
   return 0;
}

bool TechBot::isSimilarInput() const 
{
    return (TB_Input.length() > 0 && (TB_Input.find(TB_PrevInput) != std::string::npos ||
            TB_PrevInput.find(TB_Input) != std::string::npos));
}

std::string TechBot::generateSubP(Vstr wordList, size_t start, size_t end_pos)
{
    std::string buffer;
    for(int i = start; i < end_pos; ++i)
    {
        buffer += wordList[i];
        if(i != end_pos - 1)
        {
            buffer += " ";
        }
    }
    return buffer;
}

void TechBot::initPreKeyWord(std::string &str, size_t start_pos, size_t end_pos, size_t iSize)
{
    if(start_pos == 0)
    {
        str.insert(0, "_");
    }
    if(end_pos == iSize - 1)
    {
        str.insert(str.length(), "_");
    }
}

// implementing the 'sentence transposition' feature
void TechBot::changePT(std::string &str)
{
    std::string buffer = " " + str + " ";
    bool bchangePTd = 0;

    for(int i = 0; i < transposListSize; ++i)
    {
        std::string first = transposList[i].first;
        std::string second = transposList[i].second;

        while(replace(buffer, first, second) != std::string::npos)
        {
            bchangePTd = 1;
        }
    }

    if( !bchangePTd )
    {
        for(int i = 0; i < transposListSize; ++i )
        {
            std::string first = transposList[i].first;
            std::string second = transposList[i].second;
            while(replace(buffer, second, first) != std::string::npos) ;
        }
    }
    trimLR(buffer, " ");
    str = buffer;
}

// make a search for the user's input
// inside the database of the program
void TechBot::selectMatch() 
{
    std::fstream outfile("keywords.txt", std::ios::out);
    RespLISTonse.clear();
    
    // introduce thse new "string variable" to help 
    // support the implementation of keyword ranking 
    // during the matching process
    std::string bestKeyWord;
    Vstr ListOfWord;
    
    
    if(TB_Input.find("**") == std::string::npos)
    {
        RightClear(TB_Input, ".");
        ToUpperCase(TB_Input);
        SplitToWords(TB_Input, ListOfWord);
        bestKeyWord = findBestKey(ListOfWord);
        TB_KeyWord = bestKeyWord;
    }
    else
    {
        TB_KeyWord = TB_Input;
    }
    bool found=false;
    
    for(mapString ::iterator it=KnowledgeBase.begin();it!=KnowledgeBase.end();it++)
    {
        std::string temp=it->first;
        if((std::string)temp==(std::string)TB_KeyWord)
          found=true;
         
    }
        if(!found)
         {

            for(mapString ::iterator it=KnowledgeBase.begin();it!=KnowledgeBase.end();it++)
            {
                std::string temp=it->first;
                if(temp.find(TB_Input)!=std::string::npos)
                    {
                        found=true;
                    }

                
            }

        }
        if(found)
            { std::string tt=TB_KeyWord;
                std::vector<TBResponse> RespLISTObj = KnowledgeBase[tt];
                RespListgetter(RespLISTObj);
            }
            
}

std::string TechBot::findBestKey(Vstr v)
{
    std::string buffer;
    int iSize = v.size();
    bool bKeyFound = 0;
    for( int i = 0, j = iSize; i < iSize && j >= 1; ++i, --j )
    {
        for( int k = 0; (k + j) <= iSize; ++k )
        {
            buffer = generateSubP(v, k, k + j);
            if(KnowledgeBase.find(buffer) != KnowledgeBase.end())
            {
                bKeyFound = 1;
            }
            else
            {
                initPreKeyWord(buffer, k, k + j, iSize);
                if(KnowledgeBase.find(buffer) != KnowledgeBase.end())
                {
                    bKeyFound = 1;
                }
            }
            if(bKeyFound)
            {
                return buffer;
            }
        }
    }
    return buffer;
}

void TechBot::chooseOutput() 
{
    if(isAIunderstand()) 
    {
        shuffle(RespLISTonse, RespLISTonse.size());
        TB_Response = RespLISTonse[0];
    }
}

void TechBot::answer() 
{
    SavePrevresponse();
    setEvent("BOT UNDERSTAND**");
    
    if(null_input())
    {
        eventManager("NULL INPUT**");
    }
    else if(null_input_repetition())
    {
        eventManager("NULL INPUT REPETITION**");
    }
    else if(isUserRepeat())
    {
        manageUserRepeat();
    }
    else if(isaskedusername())
    {
        std::cout<<"I THINK IT IS "<<user_name<<std::endl;
        return;
    }
    else
    {
        selectMatch();
    }
    
    if(user_want_to_quit())
    {
        TB_bQuitProgram = 1;
        eventManager("USER WANTS TO QUIT**");
    }
    else if(TB_Input.find("BYE")!=std::string::npos)
        return;

   else if(!isAIunderstand())
    {
        eventManager("BOT DON'T UNDERSTAND**");
       // update_unkown_input_list();
    }
    
    if(RespLISTonse.size() > 0)
    {
        chooseOutput();
        SaveAI_response();
        initPre_response();

        if(isAIRepeat())
        {
            manageRepeat();
        }
        Savelog("TechTron");
        print_response();
        //speak(TB_Response);
    }
}
/*
void TechBot::Initialize_TTS_Engine()
{
    if(FAILED(::CoInitialize(NULL)))
    {
        throw std::string("Unable to initialise COM objects");
    }

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&TB_pVoice);

    if(FAILED(hr))
    {
        throw std::string("Unable to initialize voice engine");
    }

    TB_bTTSEngineInitialised = 1;
}

void TechBot::Release_TTS_Engine()
{
    if(TB_bTTSEngineInitialised)
    {
        TB_pVoice->Release();
        TB_pVoice = NULL;
        ::CoUninitialize();
    }
}

void TechBot::speak(const std::string text)
{
    if(TB_bTTSEngineInitialised)
    {
        WCHAR *buffer = new WCHAR[text.length() + 1];
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text.c_str(), -1, buffer, text.length() + 1);
        TB_pVoice->SetRate(-1);
        TB_pVoice->Speak(buffer, SPF_DEFAULT, NULL);
        delete [] buffer;
    }
}
*/
void TechBot::manageRepeat()
{
    std::map<int, std::string> ListOfPrevResponse;
    if(RespLISTonse.size() > 1)
    {
        std::stack<std::string> s = vResponseLog;
        Vstr::const_iterator iter = RespLISTonse.begin();
        for( ; iter != RespLISTonse.end(); ++iter )
        {
            int pos = findRespPos(*iter);
            ListOfPrevResponse[pos] = *iter;
        }
    }
    std::map<int, std::string>::const_iterator iter2 = ListOfPrevResponse.end();
    --iter2;
    TB_Response = iter2->second;
}

void TechBot::manageUserRepeat()
{
    if(isSameInput()) 
    {
        eventManager("REPETITION T1**");
    }
    else if(isSimilarInput())
    {
        eventManager("REPETITION T2**");
    }
}

void TechBot::eventManager(std::string str)
{
    SavePrevevent();
    setEvent(str);

    Saveinput();

    SetInput(str);

    if(!same_event()) 
    {
        selectMatch();
    }

    restore_input();
}

void TechBot::welcome_user()
{
	std::cout<<"What is your name?\n>";
    std::getline(std::cin,user_name);
    ToUpperCase(user_name);
    for(int i=user_name.length()-1;i>=0;i--)
    {
    	if(user_name[i]==' ')
    		{user_name=user_name.substr(i+1,user_name.length()-i+1);break;}
    }
    std::cout<<"Hi "<<user_name<<std::endl;
}

bool TechBot::isaskedusername()
{
	if(TB_Input.find("NAME?")!=std::string::npos||TB_Input.find("name?")!=std::string::npos)
    {
        return 1;
    }
    if(TB_Input.find("my name")!=std::string::npos||TB_Input.find("MY NAME?")!=std::string::npos)
    {
        return 1;
    }
    return 0;
}


int main()
{
    std::cout<<"Hi I AM TECHTRON\n";
    
    try 
    {
        TechBot AI("TechTron");
        AI.welcome_user();

        AI.welcome();
        while(!AI.quit()) 
        {
            AI.readInput();
            AI.Savelog("USER");
            if(!AI.quit())
              AI.answer();
        }
        //AI.Saveunknown_input();
    }
    catch(std::string str)
    {
        std::cerr << str << std::endl;
    }
    catch(...)
    {
        std::cerr << "The program has stop due to some unknown exception" << std::endl;
    }
    std::cout<<"LET'S TALK AGAIN "<<user_name<<std::endl;
        
    return 0;
}


