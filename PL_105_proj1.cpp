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

typedef Tree * TreePtr ; // pointer�^�ǰѼƭn�o�˫ŧi
  
int gTestNum = 0 ; // �ĴX�Ӵ��ռƾ�
 
int gLine = 1 ; // error message�� , �ĴX�C 
int gColumn = 0 ; // error message�� , �ĴX��
int gTmpLine = 1 ; // �Ȯɪ�line
int gTmpColumn = 0 ; // �Ȯɪ�Column
 
int gErrorNum = 0 ; // �P�_���Lerror, 0:no error, 1:atom or '(' expected, 2:')' expected , 3:���� , 4:EOF
int gLeft = 0 ; // �������X�� ( , �ΨӧP�_ )
int gLast = 0 ; // �����W�@��gErrorNum , �B�z����or�ťե�, �B�z���N��L�]��5
int gDot = 0 ; // �P�_�𤤪��ĤG��dot? 

int gQuoteLine = 0 ; // ��'��line
bool gQuote = false ; // �b�B�z Quote�ɵo��error�ɻݭn�� 

bool gExit = false ; // �P�_�O�_(exit)
bool gFirstInput = true ; // �P�_�O���O�Ĥ@��input 

bool gIsString = false ; // �P�_�O���O���b�B�zString
bool gIsInTree = false ; // �P�_�O���O�b���, . �O�_��error
bool gFirstPrintLeft = false ; //  �L ( �᭱���ӪF�F, �e�����n�Y�� 
bool gIsLTree = false ; // �P�_�O���O����J�� . , �p�G�O�N�����~��Ū�᭱�� 
bool gIsBehindDot = false ; // �P�_.�᭱���ĤG��token �O���O)

char gWhat = '\0' ; // �ݬOR or L or D, �k��Υ��� ��Dot 

string gErrorToken = "" ; // ��error��token
char gLastChar = '\0' ; // ��W��Ū�쪺char

char gTmpChar = '\0' ; // �B�z . () ��
string gTmpToken = "" ; // �B�z . () �� 

char GetNextChar() ; // Ū�U�@��char, ��track Line & Column 
char GetNextNonWhiteSpaceChar() ; // Ū�U�@�ӫD�ť�char, �ò��Lcomment
bool IsSeparator() ; // ��Token��
bool IsEscape( string & token ) ; // �B�z \ �� 
string GetToken( char & tmp ) ; // Ū�@��token,�æ^�Ǹ�token

void ReadSExp( TreePtr & expr, string token, char & tmp ) ; // ŪS-Exp
void PrintSExp( TreePtr & expr ) ; // �LS-Exp
void PrintErrorMessage() ; // �L�Xerror message  

bool IsInt( string & token ) ; // �P�_�O�_��Int,�Y��Int�N�ഫ�����T��token 
void IsFloat( string & token ) ; // �P�_�O�_��Float,�Y��Float�N�ഫ�����T��token
bool IsDigit( char tmp ) ; // �Ѥj������IsDigit, �u�n�ۤv�g
  
void BuildTree( TreePtr & expr, string token ) ; // �ؤ@�ʾ� 
void LeftTree( TreePtr & expr, string token, char & tmp ) ; // �����𻼰j 
void RightTree( TreePtr & expr, string token, char & tmp ) ; // �k���𻼰j 

void PrintLTree( TreePtr & expr, string whiteSpace ) ; // �L���� 
void PrintRTree( TreePtr & expr, string whiteSpace, bool printDot ) ; // �L�k��

void PrintTree( TreePtr & expr ) ; // �Ȯɥ� , �L�𪺨C�Ӹ`�I , �ˬd�𦳨S���ع� 

int main() {   
  cout << "Welcome to OurScheme!" << endl ;
  
  cin >> gTestNum ; // Ū���ռƾ� 
  
  char enter = '\0' ; // Ū���Ĥ@�Ӵ��� 
  enter = cin.get() ; // Ū���Ĥ@�Ӵ���
  
  char tmp = '\0' ; // ��token��, peek ()
  
  while ( !gExit && gErrorNum != 4 ) { // �Y����ƥi�HŪ �� input���O(exit) �N�~��     
    TreePtr expr = NULL ; // �إߤ@��node���VS-Exp��tree
    
    cout << endl << "> " ;
    
    // ��l��
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
      if ( gErrorNum == 0 ) { // �Lerror  
        PrintSExp( expr ) ; // �LS-Exp
        // PrintTree( expr ) ; // �Ȯɥ�
      } // if
      else { // ��error
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
          PrintErrorMessage() ; // �Lerror
      
        if ( gErrorNum < 3 ) { // EOF �M EOL ����Ū���F , �]���w�gŪ��檺���� 
          char read = '\0' ; // ��error�᪺���Ū�� 
          while ( read != '\n' && gErrorNum != 4 )
            read = GetNextChar() ;
        } // if
      } // else

      gLast = gErrorNum ;
    
      /*
      if ( gLast == 0 && cin.peek() == '\n' ) { // �q�U�@��}�lŪ�ϥΪ̪�input, �P��᭱���ťի��B�z!? 
        enter = GetNextChar() ;
        gLast = 5 ;
      } // if
      */
    } // if 
    
  } // while
  
  if ( gErrorNum == 4 )
    PrintErrorMessage() ; // �Lerror
  
  cout << endl << "Thanks for using OurScheme!" << endl ;
} // main()
 

char GetNextChar() { // Done
  char read = '\0' ;
  
  if ( cin.peek() != -1  ) {
    read = cin.get() ; // Ū�@�Ӧr���i�� 
  
    if ( gLastChar == '\n' ) { // �p�G�O\n �N��Line+1, Column�k�s       
      gLine++ ;
      gColumn = 1 ;
    } // if
    else // ���O\n �N��Column+1 
      gColumn++ ;
  } // if
  else {
    gErrorNum = 4 ; //  �]�wEOF error
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
  
  while ( ( read == '\n' || read == ' ' ) && cin.peek() != -1 ) { // Ū��Non-White-Space�~��  
    read = GetNextChar() ;
    
    if ( gLast == 0 && read == ';' && !gFirstInput && !gQuote ) {
      gLine-- ; 
      gLast = 5 ;
    } // if
  } // while
      
  if ( read == ';' ) { // Ū�� ; , �N�n����Ū��
    while ( cin.peek() != '\n' && cin.peek() != -1 )
      read = GetNextChar() ;      
    read = GetNextNonWhiteSpaceChar() ;
  } // if
  
  if ( cin.peek() == -1 && read != ')' ) {
    gErrorNum = 4 ; //  �]�wEOF error
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

string GetToken( char & tmp ) { // ���γB�zcomment, �]��GetNextNonWhiteSpace�N�w�g�B�z���F    
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
        gTmpLine = gLine ; // �Ȧs, �Ȧ�error 
        gTmpColumn = gColumn ; // �Ȧs, �Ȧ�error
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
          read = GetNextChar() ; // �� \ Ū��
          if ( IsEscape( token ) ) {
            read = GetNextChar() ; // ��U��Ū��
            read = '\0' ; // �òM�� 
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
        read = GetNextChar() ; // ��\nŪ��
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
            gTmpLine = gLine ; // �Ȧs, �Ȧ�error 
            gTmpColumn = gColumn ; // �Ȧs, �Ȧ�error 
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
  
  if ( !IsInt( token ) )  // �P�_�O�_��Int, �Y��Int�N�ഫ�����T��token ,�Y���OInt,�A�h�P�_�O�_��Float 
    IsFloat( token ) ; // �P�_�O�_��Float, �Y��Float�N�ഫ�����T��token 
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

void PrintSExp( TreePtr & expr ) { // �ݭק�
  if ( expr != NULL && !gExit ) {
    if ( expr->atom != "(" )
      cout << expr->atom << endl ;
    else {
      bool printDot = false ; // �n���n�LDot
      cout << expr->atom << " " ; // �L ( + �ť�*1
      gFirstPrintLeft = true ; 
      PrintLTree( expr->left, "  " ) ;
      if ( expr->right->right == NULL )
        printDot = true ;
      PrintRTree( expr->right, "  ", printDot ) ;
      cout << ")"  << endl ; // �L ) 
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
    bool printDot = false ; // �n���n�LDot
    if ( gFirstPrintLeft ) {
      cout << expr->atom << " " ; // �L ( + �ť�*1
      gFirstPrintLeft = false ;
    } // if
    else 
      cout << whiteSpace << expr->atom << " " ; // �L ( + �ť�*1
    gFirstPrintLeft = true ; 
    PrintLTree( expr->left, whiteSpace + "  " ) ;
    if ( expr->right->right == NULL )
      printDot = true ;    
    PrintRTree( expr->right, whiteSpace + "  ", printDot ) ; 
    cout << whiteSpace << ")"  << endl ; // �L ) 
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
  bool isInt = false ; // �P�_�O�_��Int
  
  if ( token[0] == '+' || token[0] == '-' || IsDigit( token[0] ) ) { // ���ˬd�Ĥ@�r���O�_���i��Ϧ�token���@��Int 
    isInt = true ; // �����]��Int, �p�G���󤣲ŦX�N�A�]��false 
    if ( token[0] == '+' || token[0] == '-' ) {
      for ( int i = 1 ; i < token.length() ; i++ ) { // ���F�Ĥ@�Ӧr�����~, �O�_�����Ʀr
        if ( !IsDigit( token[i] )  )
          isInt = false ;
      } // for

      if ( token.length() == 1 )
        isInt = false ;
    } // if
    else {
      for ( int i = 0 ; i < token.length() ; i++ ) { // �Ҧ��r��,�O�_�����Ʀr
        if ( !IsDigit( token[i] ) )
          isInt = false ;
      } // for
    } // else
  } // if  
  
  if ( isInt ) {
    stringstream ss ;
    int tmp = atoi( token.c_str() ) ; // �۰ʧ�+�� or �h�l��0�R��
    ss << tmp;
    ss >> token;
    return true ;
  } // if
  else
    return false ;
} // IsInt()

void IsFloat( string & token ) { // Done
  bool isFloat = false ; // �P�_�O�_��Float
  bool preNum = false ; // �P�_�p���I�e���O�_���Ʀr,�Y�S�����ܤ���n��0 
  bool postNum = false ; // �P�_�p���I�᭱���Ʀr�O�_��3���,�Y�S�����ܭn��0,�W�L3��ƪ��ܭn�|�ˤ��J
  bool hasDot = false ; // �P�_���L�p���I,�ΨӧP�_�O�_��Float
  
  if ( token[0] == '+' || token[0] == '-' || IsDigit( token[0] ) || token[0] == '.' ) { 
    // ���ˬd�Ĥ@�r���O�_���i��Ϧ�token���@��Float 
    isFloat = true ; // �����]��Float, �p�G���󤣲ŦX�N�A�]��false 
    if ( token[0] == '+' || token[0] == '-' ) {
      for ( int i = 1 ; i < token.length() ;  i++ ) { // �P�_���O�_���D�Ʀr && �@�Ӥp���I 
        if ( !hasDot && token[i] == '.' ) // ��p���I����m 
          hasDot = true ;
        else if ( !hasDot && IsDigit( token[i] )  ) // �]�wpreNum
          preNum = true ;
        else if ( hasDot && IsDigit( token[i] )  ) // �]�wpostNum 
          postNum = true ; 
        else if ( hasDot && !IsDigit( token[i] )  ) // �Y�����F�@�Ӥp���I���~���D�Ʀr,�N���OFloat,�]���w�g�S���P�_���t���F 
          isFloat = false ;
        else if ( !hasDot && !IsDigit( token[i] ) && token[i] != '.' )
          isFloat = false ;
      } // for
    } // if
    else if ( IsDigit( token[0] ) ) {
      preNum = true ; // preNum �]�N�Otoken[0]�w�g�O�ӼƦr�F, �ҥH�]��true  
      for ( int i = 1 ; i < token.length() ;  i++ ) { // �P�_���O�_���D�Ʀr && �@�Ӥp���I 
        if ( !hasDot && token[i] == '.' ) // ��p���I����m 
          hasDot = true ;
        else if ( hasDot && IsDigit( token[i] )  ) // �]�wpostNum 
          postNum = true ;  
        else if ( hasDot && !IsDigit( token[i] )  ) // �Y�����F�p���I���~���D�Ʀr,�N���OFloat,�]���w�g�S���P�_���t���F 
          isFloat = false ;       
      } // for
    } // else if
    else { // token[0] == '.'
      hasDot = true ; // �]��token[0]�w�g�O��dot�F, �ҥH�]��true 
      for ( int i = 1 ; i < token.length() ;  i++ ) { // �P�_���O�_���D�Ʀr && �@�Ӥp���I 
        if ( hasDot && !IsDigit( token[i] )  ) // �Y�����F�p���I���~���D�Ʀr,�N���OFloat,�]���w�g�S���P�_���t���F 
          isFloat = false ;   
        if ( hasDot && IsDigit( token[i] )  ) // �]�wpostNum 
          postNum = true ;      
      } // for 
    } // else

    if ( !hasDot || ( !preNum && !postNum ) ) // �Y���token���S��.  �� .�e�᳣�S���Ʀr , �h���OFloat 
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
            floatArray[i] = '\0' ; // �M���_�Ǧr�� 
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
    
  if ( ( token == "."  && tmp != '(' ) || token == ")" ) { // ���Ӥ��ΧP�_ ')' 
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
          token = GetToken( tmp ) ; // Ū )
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
          token = GetToken( tmp ) ; // Ū )
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
  else if ( token == ")" ) { // �ˬd�L�F, ���ݭn�A�ˬd
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
          token = GetToken( tmp ) ; // Ū (
          // BuildTree( expr, "" ) ;
          LeftTree( expr->left, "", tmp ) ;
          RightTree( expr->right, "", tmp ) ;
          
          if ( gLeft > 0 ) {
            // cout << "R -> .(" << endl ;
            token = GetToken( tmp ) ; // Ū )
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
