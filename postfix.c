/*precedence enum 수정, evaluation()내 switch구문 수정*/
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAX_STACK_SIZE 10		//STACK SIZE선언
#define MAX_EXPRESSION_SIZE 20	//수식의 크기

/* stack ë‚´ى—گى„œ ىڑ°ى„ ىˆœىœ„ëٹ” ë‚´ë¦¼ى°¨ىˆœ, lparen = 0 ê°€ى‍¥ ë‚®ى‌Œ */
typedef enum{
	lparen = 0,  /* ( 의 우선순위*/
	rparen = 9,  /* ) 의 우선순위*/
	times = 6,
	divide = 6,   /* *,/의 우선순위 */
	plus = 4,
	minus = 4,    /* +,-의 우선순위 */
	operand = 1 /* 피연산자의 우선순위 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];		//중위표기를 저장할 배열 
char postfixExp[MAX_EXPRESSION_SIZE];	//후위표기를 저장할 배열
char postfixStack[MAX_STACK_SIZE];		//후위표기로 변경하기위한 스택
int evalStack[MAX_STACK_SIZE];			//후위표기로 변경된 식을 계산하기 위한 스택

int postfixStackTop = -1;	//후위표기STACK의 TOP 위치를 가리킬 변수
int evalStackTop = -1;		//계산을위한 stack의 top위치를 가리킬 변수

int evalResult = 0;			//계산 결과값저장할 변수

void postfixPush(char x);	//후위표기로 변경하기 위한 스택에 삽입하는 함수
char postfixPop();			//스택에서 꺼낸값을 반환하는 함수
void evalPush(int x);		//evalstack에 값을 넣는다.
int evalPop();				//evalstack에서 값을 꺼낸다.
void getInfix();					//중위표기 수식 입력받는 함수
precedence getToken(char symbol);	//우선순위를 반환하는 함수
precedence getPriority(char x);		//우선순위를 반환하는 함수
void charCat(char* c);				//스택에 있는 값을 후위표기식에 이어붙이는 함수
void toPostfix();					//중위표기식을 후위표기식으로 변환하는 함수
void debug();						//디버깅
void reset();						//초기화 하는 함수
void evaluation();					//후위표기식으로 변경된 식을 계산하는 함수

int main()
{
	printf("-----[장정환] [2021076029]-----");
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();		//중위표기 수식 입력
			break;
		case 'p': case 'P':
			toPostfix();	//후위표기 수식으로 변경
			break;
		case 'e': case 'E':
			evaluation();	//후위표기로 변경후 계산 
			break;
		case 'd': case 'D':
			debug();		//디버깅
			break;
		case 'r': case 'R':
			reset();		//다른 계산을 위해 초기화
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;


}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;	//스택의 top위치를 증가시킨 곳에 값을 삽입
}

char postfixPop()
{
	char x;
    if(postfixStackTop == -1)	//공백상태일 경우
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];	//스택에서 꺼낸 값을 x에 저장후 top위치 조정
    }
    return x;	//스택의 top이 가리키는 토큰 반환
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)	//공백상태
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expressionى‌„ ى‍…ë ¥ë°›ëٹ”ë‹¤.
 * infixExpى—گëٹ” ى‍…ë ¥ëگœ ê°’ى‌„ ى €ى‍¥ي•œë‹¤.
 */

void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);	//중위표기의 수식 입력
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * ë¬¸ى‍گي•کë‚کë¥¼ ى „ë‹¬ë°›ى•„, postfixExpى—گ ى¶”ê°€
 */
void charCat(char* c)
{
	if (postfixExp == '\0')	//후위표기배열이 공백상태일 경우 배열에 값을 복사
		strncpy(postfixExp, c, 1);	
	else					//공백상태가 아닐경우 후위표기 배열에 이어붙인다
		strncat(postfixExp, c, 1);	
}

/**
 * infixExpى‌ک ë¬¸ى‍گë¥¼ ي•کë‚کى”© ى‌½ى–´ê°€ë©´ى„œ stackى‌„ ى‌´ىڑ©ي•کى—¬ postfixë،œ ë³€ê²½ي•œë‹¤.
 * ë³€ê²½ëگœ postfixëٹ” postFixExpى—گ ى €ى‍¥ëگœë‹¤.
 */
void toPostfix()
{
	/* 중위표기 수식의 위치를 exp포인터에 저장 */
	char *exp = infixExp;
	char x; /* ë¬¸ى‍گي•کë‚کë¥¼ ى‍„ى‹œë،œ ى €ى‍¥ي•کê¸° ىœ„ي•œ ë³€ىˆک */

	/* expë¥¼ ى¦‌ê°€ى‹œى¼œê°€ë©´ى„œ, ë¬¸ى‍گë¥¼ ى‌½ê³  postfixë،œ ë³€ê²½ */
	while(*exp != '\0')
	{	//전위표기식의 마지막 값까지 반복
		if(getPriority(*exp) == operand)	//피연산자일 경우
		{
			x = *exp;
        	charCat(&x);
		}
        else if(getPriority(*exp) == lparen) {	//'('일 경우

        	postfixPush(*exp);	//스택에 삽입
        }
        else if(getPriority(*exp) == rparen)	//')'일 경우
        {
        	while((x = postfixPop()) != '(') {	//'('가 나올때 까지 스택에 있는 값을 꺼내 후위표기배열에 저장
        		charCat(&x);
        	}
        }
        else
        {
            while(getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))	
            {	//전위 표기의 토큰보다 스택의 마지막에 있는 토큰의 우선순위가 크거나 같은 동안 반복
            	x = postfixPop();
            	charCat(&x);	//스택에서 꺼낸 연산자를 후위표기배열에 이어붙인다.
            }
            postfixPush(*exp);	//스택에 삽입
        }
        exp++;		//수식의 다음위치로 조정
	}

    while(postfixStackTop != -1)
    {//스택에 있는 나머지 값을 후위표기배열에 이어붙인다.
    	x = postfixPop();
    	charCat(&x);
    }

}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);		//전위표기식 출력
	printf("postExp =  %s\n", postfixExp);		//후위표기식 출력
	printf("eval result = %d\n", evalResult);	//계산결과 출력

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);		//후위표기를 위한 스택 출력

	printf("\n");

}

void reset()
{	//전위표기와 후위표기 배열 초기화
	infixExp[0] = '\0';		
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';		//스택 초기화
        
	//top과 결과값 초기화
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}
void evaluation()
{
	int opr1, opr2, i;

	int length = strlen(postfixExp);	//후위표기 배열의 길이를 length에 저장
	char symbol;
	evalStackTop = -1;					//계산 결과값을 위한 스택의 원소를 가리킬 top변수

	for(i = 0; i < length; i++)	
	{
		symbol = postfixExp[i];			//후위표기 배열 i번째 값을 symbol에 저장
		if(getToken(symbol) == operand) {	//피연산자일경우 
			evalPush(symbol - '0');			//문자symbol의 값을 한자리 정수로 변환하여 스택에 넣는다.
		}
		else {	//연산자일경우
			opr2 = evalPop();	//스택에서 두개의 피연산자를 꺼낸다.
			opr1 = evalPop();
			switch(symbol) {	//계산한값을 스택에 넣는다.
			case '+': evalPush(opr1 + opr2); break;
			case '-': evalPush(opr1 - opr2); break;
			case '*': evalPush(opr1 * opr2); break;
			case '/': evalPush(opr1 / opr2); break;
			default: break;
			}
		}
	}
	evalResult = evalPop();	//계산 결과 값을 저장
}
