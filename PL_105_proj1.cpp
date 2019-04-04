# include <iostream>
# include <string>
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <sstream>

using namespace std ;

struct Tree { 
  string atom ;
  Tree * left ;  
  Tree * right ;
};

typedef Tree * TreePtr ; // pointer回傳參數要這樣宣告
  
int gTestNum = 0 ; // 第幾個測試數據
 
int gLine = 1 ; // error message用 , 第幾列 
int gColumn = 0 ; // error message用 , 第幾行
int gTmpLine = 1 ; // 暫時的line
int gTmpColumn = 0 ; // 暫時的Column
 
int gErrorNum = 0 ; // 判斷有無error, 0:no error, 1:atom or '(' expected, 2:')' expected , 3:換行 , 4:EOF
int gLeft = 0 ; // 紀錄有幾個 ( , 用來判斷 )
int gLast = 0 ; // 紀錄上一個gErrorNum , 處理換行or空白用, 處理完就把他設為5
int gDot = 0 ; // 判斷樹中的第二個dot? 

int gQuoteLine = 0 ; // 放'的line
bool gQuote = false ; // 在處理 Quote時發生error時需要用 

bool gExit = false ; // 判斷是否(exit)
bool gFirstInput = true ; // 判斷是不是第一次input 

bool gIsString = false ; // 判斷是不是正在處理String
bool gIsInTree = false ; // 判斷是不是在樹裡, . 是否為error
bool gFirstPrintLeft = false ; //  印 ( 後面那個東東, 前面不要縮排 
bool gIsLTree = false ; // 判斷是不是左樹遇到 . , 如果是就不用繼續讀後面的 
bool gIsBehindDot = false ; // 判斷.後面的第二個token 是不是)

char gWhat = '\0' ; // 看是R or L or D, 右樹或左樹 或Dot 

string gErrorToken = "" ; // 放error的token
char gLastChar = '\0' ; // 放上個讀到的char

char gTmpChar = '\0' ; // 處理 . () 用
string gTmpToken = "" ; // 處理 . () 用 

char GetNextChar() ; // 讀下一個char, 並track Line & Column 
char GetNextNonWhiteSpaceChar() ; // 讀下一個非空白char, 並略過comment
bool IsSeparator() ; // 切Token用
bool IsEscape( string & token ) ; // 處理 \ 用 
string GetToken( char & tmp ) ; // 讀一個token,並回傳該token

void ReadSExp( TreePtr & expr, string token, char & tmp ) ; // 讀S-Exp
void PrintSExp( TreePtr & expr ) ; // 印S-Exp
void PrintErrorMessage() ; // 印出error message  

bool IsInt( string & token ) ; // 判斷是否為Int,若為Int就轉換成正確的token 
void IsFloat( string & token ) ; // 判斷是否為Float,若為Float就轉換成正確的token
bool IsDigit( char tmp ) ; // 老大不給用IsDigit, 只好自己寫
  
void BuildTree( TreePtr & expr, string token ) ; // 建一棵樹 
void LeftTree( TreePtr & expr, string token, char & tmp ) ; // 左指樹遞迴 
void RightTree( TreePtr & expr, string token, char & tmp ) ; // 右指樹遞迴 

void PrintLTree( TreePtr & expr, string whiteSpace ) ; // 印左樹 
void PrintRTree( TreePtr & expr, string whiteSpace, bool printDot ) ; // 印右樹

void PrintTree( TreePtr & expr ) ; // 暫時用 , 印樹的每個節點 , 檢查樹有沒有建對 

int main() {   
  cout << "Welcome to OurScheme!" << endl ;
  
  cin >> gTestNum ; // 讀測試數據 
  
  char enter = '\0' ; // 讀掉第一個換行 
  enter = cin.get() ; // 讀掉第一個換行
  
  char tmp = '\0' ; // 切token用, peek ()
  
  while ( !gExit && gErrorNum != 4 ) { // 若有資料可以讀 或 input不是(exit) 就繼續做     
    TreePtr expr = NULL ; // 建立一個node指向S-Exp之tree
    
    cout << endl << "> " ;
    
    // 初始化
    gLine = 1 ; 
    gColumn = 0 ;
    gLeft = 0 ;
    gLastChar = '\0' ; 
    gErrorToken = "" ;
    gIsString = false ;
    gIsInTree = false ;
    gQuote = false ;
    gIsLTree = false ;
    gIsBehindDot = false ; 
    gErrorNum = 0 ;
    gDot = 0 ;
    gWhat = '\0' ; 
    
    try {
      ReadSExp( expr, "", tmp ) ;
      gFirstInput = false ;
      
      if ( expr->left != NULL && expr->left->atom == "exit" && 
           expr->right != NULL && expr->right->atom == "nil" )
        gExit = true ;
    } catch ( int num ) {
    } // catch

    if ( !gExit ) {
      if ( gErrorNum == 0 ) { // 無error  
        PrintSExp( expr ) ; // 印S-Exp
        // PrintTree( expr ) ; // 暫時用
      } // if
      else { // 有error
        if ( ( gErrorToken == "." || gErrorToken == "(" ) && gIsInTree && !gIsLTree ) {
          if ( gDot != 2 && !gIsBehindDot ) {
            if ( gErrorToken == "." && gIsInTree ) {
              gLine = gTmpLine ;
              gColumn = gTmpColumn ;
            } // if
            else if ( gLine == gTmpLine ) {
              gColumn = gTmpColumn ;
            } // else if
          } // if
          
          tmp = '\0' ;
          gTmpChar = '\0' ;
          gTmpToken = "" ;
        } // if
        
        if ( gErrorNum != 4 )
          PrintErrorMessage() ; // 印error
      
        if ( gErrorNum < 3 ) { // EOF 和 EOL 不用讀掉了 , 因為已經讀到行的底部 
          char read = '\0' ; // 整error後的整行讀掉 
          while ( read != '\n' && gErrorNum != 4 )
            read = GetNextChar() ;
        } // if
      } // else

      gLast = gErrorNum ;
    
      /*
      if ( gLast == 0 && cin.peek() == '\n' ) { // 從下一行開始讀使用者的input, 同行後面的空白怎麼處理!? 
        enter = GetNextChar() ;
        gLast = 5 ;
      } // if
      */
    } // if 
    
  } // while
  
  if ( gErrorNum == 4 )
    PrintErrorMessage() ; // 印error
  
  cout << endl << "Thanks for using OurScheme!" << endl ;
} // main()
 

char GetNextChar() { // Done
  char read = '\0' ;
  
  if ( cin.peek() != -1  ) {
    read = cin.get() ; // 讀一個字元進來 
  
    if ( gLastChar == '\n' ) { // 如果是\n 就把Line+1, Column歸零       
      gLine++ ;
      gColumn = 1 ;
    } // if
    else // 不是\n 就把Column+1 
      gColumn++ ;
  } // if
  else {
    gErrorNum = 4 ; //  設定EOF error
    // throw gErrorNum ;
  } // else
  
  // cout << "Read: " << read << endl ; 
  // cout << "Line: " << gLine << " Column: " << gColumn << endl ;
  // cout << "Last: " << gLast << endl ; 
  gLastChar = read ;
  return read ;
} // GetNextChar()
 
char GetNextNonWhiteSpaceChar() { // Done
  char read = GetNextChar() ; 
  
  while ( ( read == '\n' || read == ' ' ) && cin.peek() != -1 ) { // 讀到Non-White-Space才停  
    read = GetNextChar() ;
    
    if ( gLast == 0 && read == ';' && !gFirstInput && !gQuote ) {
      gLine-- ; 
      gLast = 5 ;
    } // if
  } // while
      
  if ( read == ';' ) { // 讀到 ; , 就要把整行讀掉
    while ( cin.peek() != '\n' && cin.peek() != -1 )
      read = GetNextChar() ;      
    read = GetNextNonWhiteSpaceChar() ;
  } // if
  
  if ( cin.peek() == -1 && read != ')' ) {
    gErrorNum = 4 ; //  設定EOF error
    // throw gErrorNum ;
  } // if
  
  // cout << "Non_Read: " << read << endl ; 
  // cout << "Line: " << gLine << " Column: " << gColumn << endl ;
  return read ;
} // GetNextNonWhiteSpaceChar()

bool IsSeparator() { // Done
  if ( cin.peek() == ' ' || cin.peek() == '(' || cin.peek() == ')' ||
       cin.peek() == '\"' || cin.peek() == '\'' || cin.peek() == '\n' ||
       cin.peek() == ';' || cin.peek() == -1 )
    return true ;
  else
    return false ; 
} // IsSeparator()

bool IsEscape( string & token ) { // Done 
  if ( cin.peek() == 'n' || cin.peek() == 't' || cin.peek() == '\"' || cin.peek() == '\\' ) {
    if ( cin.peek() == 'n' )
      token = token + '\n' ;
    else if ( cin.peek() == 't' )
      token = token + '\t' ;
    else if ( cin.peek() == '\"' )
      token = token + '\"' ;
    else
      token = token + '\\' ;
    return true ;
  } // if 
  else
    return false ;
} // IsEscape()

string GetToken( char & tmp ) { // 不用處理comment, 因為GetNextNonWhiteSpace就已經處理掉了    
  string token = "" ;
  char read = '\0' ;
  
  // cout << "In Token: " << token << "Tmp: " << tmp << "ErrorNum: " << gErrorNum << endl ; 
  
  if ( gTmpToken == "" ) {   
    if ( tmp == '\0' )
      read = GetNextNonWhiteSpaceChar() ;
    else {
      read = tmp ;
      tmp = '\0' ;
    } // else
    
    if ( gTmpChar != '\0' ) {
      tmp = gTmpChar ;
      gTmpChar = '\0' ;
    } // if
    
    if ( gErrorNum == 4 )
      throw gErrorNum ;
      
    if ( gLast == 0 && gLine > 1 )
      gLine-- ;
    gLast = 5 ;
    
    if ( read == '(' ) {
      token = read ;
      if ( !gIsBehindDot ) {
        gTmpLine = gLine ; // 暫存, 怕有error 
        gTmpColumn = gColumn ; // 暫存, 怕有error
        if ( tmp == '\0' )
          tmp = GetNextNonWhiteSpaceChar() ;
        
        if ( tmp == ')' ) {
          token = token + tmp ;
          tmp = '\0' ;
        } // if
        else 
          gLeft++ ;
      } // if
       
    } // if
    else if ( read == ')' ) {
      if ( gLeft == 0 || !gIsInTree ) {
        gErrorToken = gErrorToken + read ;
        gErrorNum = 1 ;
        throw gErrorNum ;
      } // if
      else {
        token = read ;
        gLeft-- ;
      } // else
    } // else if
    else if ( read == '\"' ) {
      gIsString = true ;
      token = read ;
      read = '\0' ;
      while ( gErrorNum != 4 && cin.peek() != '\n' && read != '\"' ) {
        if ( cin.peek() == '\\' ) {
          read = GetNextChar() ; // 把 \ 讀掉
          if ( IsEscape( token ) ) {
            read = GetNextChar() ; // 把下個讀掉
            read = '\0' ; // 並清空 
          } // if
          else {
            token = token + read ;
          } // else 
        } // if
        else {
          read = GetNextChar() ;
          token = token + read ;
        } // else 
      } // while
        
      if ( cin.peek() == '\n' &&  read != '\"' ) {
        read = GetNextChar() ; // 把\n讀掉
        gErrorNum = 3 ;
        throw gErrorNum ;
      } // if
          
    } // else if
    else if ( read == '.' ) {
      if ( IsSeparator() ) {
        if ( !gIsInTree ) {
          gErrorToken = gErrorToken + read ;
          gErrorNum = 1 ;
          throw gErrorNum ;
        } // if
        else {
          token = read ;
          
          if ( gIsLTree ) {
            gErrorToken = gErrorToken + read ;
            gErrorNum = 1 ;
            throw gErrorNum ;
          } // if
          
          if ( !gIsBehindDot ) {
            gTmpLine = gLine ; // 暫存, 怕有error 
            gTmpColumn = gColumn ; // 暫存, 怕有error 
            if ( tmp == '\0' )
              tmp = GetNextNonWhiteSpaceChar() ;
            
            if ( tmp == '(' ) {
              gTmpChar = GetNextNonWhiteSpaceChar() ;
              if ( gTmpChar == ')' ) {
                gTmpToken = "nil" ;
                tmp = '\0' ;
                gTmpChar = '\0' ; 
              } // if
            } // if
          } // if
        } // else
      } // if
      else {
        token = read ;
        while ( gErrorNum != 4 && !IsSeparator() ) 
          token = token + GetNextChar() ; 
      } // else
    } // else if
    else if ( read == '\'' ) {
      gQuoteLine = gLine ;
      token = read ;
    } // else if
    else {
      token = read ;
      while ( gErrorNum != 4 && !IsSeparator() ) 
        token = token + GetNextChar() ;
    } // else
  } // if
  else {
    token = gTmpToken ; 
    gTmpToken = "" ;
  } // else
  
  if ( !IsInt( token ) )  // 判斷是否為Int, 若為Int就轉換成正確的token ,若不是Int,再去判斷是否為Float 
    IsFloat( token ) ; // 判斷是否為Float, 若為Float就轉換成正確的token 
  if ( token == "nil" || token == "#f" || token == "()" ) 
    token = "nil" ;
  else if ( token == "#t" || token == "t" )
    token = "#t" ;
  
  // cout << "Out Token: " << token << " Tmp: " << tmp << " gTmpChar: " << gTmpChar << endl ; 
  // cout << " ErrorNum: " << gErrorNum << endl ; 
  // cout << "Line: " << gLine << " Column: " << gColumn << endl ;
  // cout << "Left: "  << gLeft << " Token: " << token<< endl;
  
  // cout << "Last: " << gLast << endl ;  
  
  if ( token == "." && tmp == '.' && IsSeparator() ) {
    gDot = 2 ;
    gErrorToken = "." ;
    gErrorNum = 1 ;
    throw gErrorNum ;
  } // if
  
  return token ; 
} // GetToken()

void ReadSExp( TreePtr & expr, string token, char & tmp ) {
  if ( token == "" ) {
    token = GetToken( tmp ) ;
    if ( gLast == 0 && gLine > 1 )
      gLine-- ;
    gLast = 5 ;
  } // if
    
    
  if ( token == "(" ) {
    gIsInTree = true ;
    BuildTree( expr, token ) ;
    LeftTree( expr->left, "", tmp ) ; 
    RightTree( expr->right, "", tmp ) ;
  } // if
  else if ( token == "\'" ) {
    gQuote = true ; 
    if ( gWhat == 'D' ) {
      gIsInTree = true ;
      BuildTree( expr, "" ) ;
      BuildTree( expr->left, "quote" ) ;
      BuildTree( expr->right, "" ) ;
      LeftTree( expr->right->left, "", tmp ) ;
      BuildTree( expr->right->right, "nil" ) ; 
    } // if
    else if ( gWhat == 'R' ) {
      gIsInTree = true ;
      BuildTree( expr, "" ) ;
      BuildTree( expr->left, "(" ) ;
      BuildTree( expr->left->left, "quote" ) ;
      BuildTree( expr->left->right, "" ) ;
      LeftTree( expr->left->right->left, "", tmp ) ;
      // cout << "R Left Done" << endl ;  
      BuildTree( expr->left->right->right, "nil" ) ; 
      // BuildTree( expr->right, "nil" ) ;
      RightTree( expr->right, "", tmp ) ;
    } // else if
    else { // \0 or L
      gIsInTree = true ;
      BuildTree( expr, "(" ) ;
      BuildTree( expr->left, "quote" ) ;
      BuildTree( expr->right, "" ) ;
      LeftTree( expr->right->left, "", tmp ) ;
      // cout << "L Left Done" << endl ; 
      BuildTree( expr->right->right, "nil" ) ; 
    } // else
    
    gQuote = false ;
  } // else if
  else {
    BuildTree( expr, token ) ; 
  } // else
} // ReadSExp()   

void PrintSExp( TreePtr & expr ) { // 待修改
  if ( expr != NULL && !gExit ) {
    if ( expr->atom != "(" )
      cout << expr->atom << endl ;
    else {
      bool printDot = false ; // 要不要印Dot
      cout << expr->atom << " " ; // 印 ( + 空白*1
      gFirstPrintLeft = true ; 
      PrintLTree( expr->left, "  " ) ;
      if ( expr->right->right == NULL )
        printDot = true ;
      PrintRTree( expr->right, "  ", printDot ) ;
      cout << ")"  << endl ; // 印 ) 
    } // else
  } // if
} // PrintSExp()

void PrintTree( TreePtr & expr ) {
  if ( expr != NULL ) {
    cout << expr->atom << endl ;
    PrintTree( expr->left ) ;
    PrintTree( expr->right ) ;
  } // if
} // PrintTree() 

void PrintLTree( TreePtr & expr, string whiteSpace ) {
  if ( expr->atom == "(" ) {
    bool printDot = false ; // 要不要印Dot
    if ( gFirstPrintLeft ) {
      cout << expr->atom << " " ; // 印 ( + 空白*1
      gFirstPrintLeft = false ;
    } // if
    else 
      cout << whiteSpace << expr->atom << " " ; // 印 ( + 空白*1
    gFirstPrintLeft = true ; 
    PrintLTree( expr->left, whiteSpace + "  " ) ;
    if ( expr->right->right == NULL )
      printDot = true ;    
    PrintRTree( expr->right, whiteSpace + "  ", printDot ) ; 
    cout << whiteSpace << ")"  << endl ; // 印 ) 
  } // if
  else {
    if ( gFirstPrintLeft ) {
      cout << expr->atom << endl ;
      gFirstPrintLeft = false ; 
    } // if
    else
      cout << whiteSpace << expr->atom << endl ;
  } // else
} // PrintLTree()

void PrintRTree( TreePtr & expr, string whiteSpace, bool printDot ) {
  if ( expr->atom == "" ) {
    PrintLTree( expr->left, whiteSpace ) ;
    if ( expr->right->right == NULL )
      printDot = true ;   
    PrintRTree( expr->right, whiteSpace, printDot ) ;     
  } // if
  else {
    if ( printDot ) {
      if ( expr->atom != "nil" ) {
        cout << whiteSpace << "." << endl ;
        cout << whiteSpace << expr->atom << endl ;
      } // if 
    } // if
    else {
      cout << whiteSpace << expr->atom << endl ;
    } // else
  } // else
  
} // PrintRTree()

void PrintErrorMessage() { // Done
  if ( gErrorNum == 1 )
    cout << "ERROR (unexpected token) : atom or '(' expected when token at Line " << gLine  
         << " Column " << gColumn << " is >>" << gErrorToken << "<<" << endl ;
  else if ( gErrorNum == 2 )
    cout << "ERROR (unexpected token) : ')' expected when token at Line " << gLine  
         << " Column " << gColumn << " is >>" << gErrorToken << "<<" << endl ;
  else if ( gErrorNum == 3 )
    cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << gLine  
         << " Column " << gColumn << endl ; 
  else if ( gErrorNum == 4 )
    cout << "ERROR (no more input) : END-OF-FILE encountered" ;
} // PrintErrorMessage()

bool IsInt( string & token ) { // Done
  bool isInt = false ; // 判斷是否為Int
  
  if ( token[0] == '+' || token[0] == '-' || IsDigit( token[0] ) ) { // 先檢查第一字元是否有可能使此token為一個Int 
    isInt = true ; // 先假設為Int, 如果條件不符合就再設為false 
    if ( token[0] == '+' || token[0] == '-' ) {
      for ( int i = 1 ; i < token.length() ; i++ ) { // 除了第一個字元之外, 是否都為數字
        if ( !IsDigit( token[i] )  )
          isInt = false ;
      } // for

      if ( token.length() == 1 )
        isInt = false ;
    } // if
    else {
      for ( int i = 0 ; i < token.length() ; i++ ) { // 所有字元,是否都為數字
        if ( !IsDigit( token[i] ) )
          isInt = false ;
      } // for
    } // else
  } // if  
  
  if ( isInt ) {
    stringstream ss ;
    int tmp = atoi( token.c_str() ) ; // 自動把+號 or 多餘的0刪除
    ss << tmp;
    ss >> token;
    return true ;
  } // if
  else
    return false ;
} // IsInt()

void IsFloat( string & token ) { // Done
  bool isFloat = false ; // 判斷是否為Float
  bool preNum = false ; // 判斷小數點前面是否有數字,若沒有的話之後要補0 
  bool postNum = false ; // 判斷小數點後面的數字是否有3位數,若沒有的話要補0,超過3位數的話要四捨五入
  bool hasDot = false ; // 判斷有無小數點,用來判斷是否為Float
  
  if ( token[0] == '+' || token[0] == '-' || IsDigit( token[0] ) || token[0] == '.' ) { 
    // 先檢查第一字元是否有可能使此token為一個Float 
    isFloat = true ; // 先假設為Float, 如果條件不符合就再設為false 
    if ( token[0] == '+' || token[0] == '-' ) {
      for ( int i = 1 ; i < token.length() ;  i++ ) { // 判斷後方是否有非數字 && 一個小數點 
        if ( !hasDot && token[i] == '.' ) // 找小數點的位置 
          hasDot = true ;
        else if ( !hasDot && IsDigit( token[i] )  ) // 設定preNum
          preNum = true ;
        else if ( hasDot && IsDigit( token[i] )  ) // 設定postNum 
          postNum = true ; 
        else if ( hasDot && !IsDigit( token[i] )  ) // 若有除了一個小數點之外的非數字,就不是Float,因為已經沒有判斷正負號了 
          isFloat = false ;
        else if ( !hasDot && !IsDigit( token[i] ) && token[i] != '.' )
          isFloat = false ;
      } // for
    } // if
    else if ( IsDigit( token[0] ) ) {
      preNum = true ; // preNum 也就是token[0]已經是個數字了, 所以設為true  
      for ( int i = 1 ; i < token.length() ;  i++ ) { // 判斷後方是否有非數字 && 一個小數點 
        if ( !hasDot && token[i] == '.' ) // 找小數點的位置 
          hasDot = true ;
        else if ( hasDot && IsDigit( token[i] )  ) // 設定postNum 
          postNum = true ;  
        else if ( hasDot && !IsDigit( token[i] )  ) // 若有除了小數點之外的非數字,就不是Float,因為已經沒有判斷正負號了 
          isFloat = false ;       
      } // for
    } // else if
    else { // token[0] == '.'
      hasDot = true ; // 因為token[0]已經是個dot了, 所以設為true 
      for ( int i = 1 ; i < token.length() ;  i++ ) { // 判斷後方是否有非數字 && 一個小數點 
        if ( hasDot && !IsDigit( token[i] )  ) // 若有除了小數點之外的非數字,就不是Float,因為已經沒有判斷正負號了 
          isFloat = false ;   
        if ( hasDot && IsDigit( token[i] )  ) // 設定postNum 
          postNum = true ;      
      } // for 
    } // else

    if ( !hasDot || ( !preNum && !postNum ) ) // 若整個token中沒有.  或 .前後都沒有數字 , 則不是Float 
      isFloat = false ;

    if ( isFloat ) {
      double tmp = atof( token.c_str() ) ;
      if ( tmp == 0 )
        token = "0.000" ;
      else {
        int len = token.length() + 5 ;
        char floatArray[len] ;
        int num = 0 ; 
        bool dot = false ;
        token = "";
        sprintf( floatArray, "%.3f", tmp ) ; // float to char array
        for ( int i = 0 ; i < sizeof( floatArray ) ; i++ ) {
          if ( floatArray[i] == '.' && !dot )
            dot = true ;
          else if ( !IsDigit( floatArray[i] ) && dot && i != 0 ) 
            floatArray[i] = '\0' ; // 清除奇怪字元 
          else if ( IsDigit( floatArray[i] ) && dot )
            num++ ;
            
          token = token + floatArray[i] ;
          
          if ( num == 3 )
            i = sizeof( floatArray ) ;          
        } // for 
      } // else
    } // if
  } // if
} // IsFloat()

bool IsDigit( char tmp ) { // Done 
  if ( tmp == '0' || tmp == '1' || tmp == '2' || tmp == '3' || tmp == '4' || 
       tmp == '5' || tmp == '6' || tmp == '7' || tmp == '8' || tmp == '9' )
    return true ;
  else
    return false ;
} // IsDigit()
 
void BuildTree( TreePtr & expr, string token ) { 
  // cout << "BTree Left: " << gLeft << " Token: " << token << endl ; 
  if ( expr == NULL ) {
    expr = new Tree ;
    expr->atom = token ;
    expr->left = NULL ;
    expr->right = NULL ;
  } // if
} // BuildTree()

void LeftTree( TreePtr & expr, string token, char & tmp ) {
  gIsLTree = true ;
  if ( token == "" )
    token = GetToken( tmp ) ;
    
  // cout << "LTree Left: " << gLeft << " Token: " << token << " Tmp: " << tmp  << endl ; 
  // cout << " gTmpChar: " << gTmpChar << endl ;  
    
  if ( ( token == "."  && tmp != '(' ) || token == ")" ) { // 應該不用判斷 ')' 
    gErrorNum = 1 ;
    gErrorToken = token ;
    throw gErrorNum ;
  } // if
  else {
    if ( token == "\'" ) {
      gWhat = 'L' ;
      ReadSExp( expr, token, tmp ) ;
    } // if
      
    else {
      BuildTree( expr, token ) ; 
      if ( token == "(" ) {
        LeftTree( expr->left, "", tmp ) ;
        RightTree( expr->right, "", tmp ) ;
      } // if
    } // else
  } // else
} // LeftTree() 

void RightTree( TreePtr & expr, string token, char & tmp ) {
  gIsLTree = false ;
  if ( token == "" )
    token = GetToken( tmp ) ;
    
  // cout << "RTree Left: " << gLeft << " Token: " << token << " Tmp: " << tmp << endl ;
  // cout << " gTmpChar: " << gTmpChar << endl ;
    
  if ( token == "." && tmp != '(' ) {
    token = GetToken( tmp ) ; 
    if ( token != ")" ) {
      if ( token == "\'" ) {
        gWhat = 'D' ;
        ReadSExp( expr, token, tmp ) ;
        
        
        if ( gLeft > 0 ) {
          // cout << "R -> . -> \'" << endl ;
          token = GetToken( tmp ) ; // 讀 )
          if ( token != ")" ) {
            gErrorNum = 2 ;
            gErrorToken = token ;
            gColumn = gColumn - ( token.length()-1 ) ;
            throw gErrorNum ;
          } // if
        } // if
        
      } // if
      else if ( token == "." ) {
        gErrorNum = 1 ;
        gErrorToken = token ;
        gColumn = gColumn ;
        throw gErrorNum ;
      } // else if
      else {
        BuildTree( expr, token ) ;        
        
        if ( gLeft > 0 ) {
          // cout << "R -> ." << endl ;
          gIsBehindDot = true ;
          token = GetToken( tmp ) ; // 讀 )
          if ( token != ")" ) {
            gErrorNum = 2 ;
            gErrorToken = token ;
            gColumn = gColumn - ( token.length()-1 ) ;
            throw gErrorNum ;
          } // if
          
          gIsBehindDot = false ;
        } // if
      } // else
    } // if
    else {
      gErrorNum = 1 ;
      gErrorToken = token ;
      gColumn = gColumn ;
      throw gErrorNum ;
    } // else
  } // if
  else if ( token == ")" ) { // 檢查過了, 不需要再檢查
    // cout << "R -> nil" << endl ;  
    BuildTree( expr, "nil" ) ;
  } // else if
  else { // atom or .(
    if ( token == "\'" ) {
      gWhat = 'R' ;
      ReadSExp( expr, token, tmp ) ;
    } // if
    else {
      BuildTree( expr, "" ) ; 
      if ( token == "(" ) {
        LeftTree( expr->left, token, tmp ) ;
        RightTree( expr->right, "", tmp ) ;
      } // if
      else {
        if ( token == "." && tmp == '(' ) {
          token = GetToken( tmp ) ; // 讀 (
          // BuildTree( expr, "" ) ;
          LeftTree( expr->left, "", tmp ) ;
          RightTree( expr->right, "", tmp ) ;
          
          if ( gLeft > 0 ) {
            // cout << "R -> .(" << endl ;
            token = GetToken( tmp ) ; // 讀 )
            if ( token != ")" ) {
              gErrorNum = 2 ;
              gErrorToken = token ;
              gColumn = gColumn - ( token.length()-1 ) ;
              throw gErrorNum ;
            } // if
          } // if
        } // if
        else {
          LeftTree( expr->left, token, tmp ) ;
          RightTree( expr->right, "", tmp ) ;
        } // else
      } // else
    } // else
  } // else
} // RightTree() 
