/*동적할당받은 큐를 해제하도록 수정*/
#include <stdio.h>
#include <stdlib.h>

/*만원과 공백상태를 구분하기위해 원소를 한칸비우므로, 
실질적으로 저장할 원소개수는 MAX_QUEUE_SIZE-1개*/
#define MAX_QUEUE_SIZE 4	

typedef char element;	
typedef struct {		//MAX_QUEUE_SIZE의 원소를 가진 queue
	element queue[MAX_QUEUE_SIZE];
	int front, rear;	//원형 큐의 원소를 삭제할 위치와 추가할 위치를 나타낸는 변수
}QueueType;


QueueType *createQueue();		//원형큐를 동적할당하여 주소반환하는 함수
int freeQueue(QueueType *cQ);	//동적할당한 원형 큐 해제해주는 함수
int isEmpty(QueueType *cQ);		//큐가 공백상태인지 확인하는 함수
int isFull(QueueType *cQ);		//큐가 만원상태인지 확인하는 함수
void enQueue(QueueType *cQ, element item);	//큐에 원소를 삽입하는 함수
void deQueue(QueueType *cQ, element* item);	//큐에 있는 원소를 제거하는 함수
void printQ(QueueType *cQ);		//큐를 출력하는 함수
void debugQ(QueueType *cQ);		//element들의 위치를 출력하여 printQ와 비교하는 함수
element getElement();			//사용자로부터 원소를 입력받는 함수


int main(void)
{
	printf("-----[장정환] [2021076029]-----");
	QueueType *cQ = createQueue();	//큐 생성
	element data;

	char command;

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			//입력받은 원소를 원형큐에 추가
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			//front의 위치를 조정하여 data는 그대로 있지만 빈공간으로 인식하게 한다.
			deQueue(cQ,&data);
			
			break;
		case 'p': case 'P':
			//큐 출력
			printQ(cQ);
			break;
		case 'b': case 'B':
			//인덱스 값을 출력하여 확인한다.
			debugQ(cQ);
			break;
		case 'q': case 'Q':
			//힙공간에 동적할당받은 원형큐 해제
			freeQueue(cQ);	
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');	//사용자로부터 q,Q값을 입력받을 동안 반복


	return 1;
}

QueueType *createQueue()	//원형큐를 동적할당하여 그 주소를 반환하는 함수
{
	//동적할당한 QueueType을 가리킬 포인터
	QueueType *cQ;	
	//힙공간에 QueueType만큼을 동적할당
	cQ = (QueueType *)malloc(sizeof(QueueType));
	//원형큐의 삭제,추가위치를 가리키는 변수 초기화
	cQ->front = 0;
	cQ->rear = 0;
	//동적할당된 주소 반환
	return cQ;
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;	//동적할당 된 주소 값이 없을 경우 return 1
    free(cQ);		//동적할당 해제
    return 1;
}

element getElement()	//원형큐에 추가할 element를 입력받아 반환하는 함수
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;	//입력받은 element 반환
}


int isEmpty(QueueType *cQ)	//원형큐가 공백상태경우 1반환
{
	if (cQ->front == cQ->rear){	//front와 rear가 같을경우 공백상태
		printf("Circular Queue is empty!");
		return 1;
	}
	else return 0;
}

int isFull(QueueType *cQ)	//원형큐가 만원상태일경우 1반환
{
	//rear를 한개증가시켜 MAXSIZE로 나눈 나머지 값과 front의 값이 같을 경우 만원
	if (((cQ->rear+1)%MAX_QUEUE_SIZE) == cQ->front) { 
		printf(" Circular Queue is full!");
		return 1;
	}
	else return 0;
}

void enQueue(QueueType *cQ, element item)
{
	if(isFull(cQ)) return;	//원형큐가 MAX_QUEUE_SIZE-1개수의 원소들로 꽉 찼을 경우
	else {	//원형큐에 공간이 남아있을경우 
		cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;	//rear위치 조정
		cQ->queue[cQ->rear] = item;	//rear위치에 item 삽입
	}
}

void deQueue(QueueType *cQ, element *item)
{
	if(isEmpty(cQ)) return;	//원형 큐가 비어있을경우 
	else {
		cQ->front = (cQ->front + 1)%MAX_QUEUE_SIZE;	//front위치 조정
		//delete될값이 사용될 경우를 대비해 간접참조로 data를 front가 가리키는 값으로 변경 
		//front가 가리키는 값이 삭제되는것은 아니지만 printQ함수에서 출력이 안되거나 나중에 원소가 덮어씌어진다.
		*item = cQ->queue[cQ->front];		
		return;
	}
}


void printQ(QueueType *cQ)	//원형큐의 element들을 출력하는 함수
{
	int i, first, last; 

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;	//원형 큐 시작위치
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;	//원형 큐 출력 마지막위치(빈공간)

	printf("Circular Queue : [");
	i = first;
	while(i != last){	//원형큐의 마지막 원소 까지 출력
		printf("%3c", cQ->queue[i]);
		i = (i+1)%MAX_QUEUE_SIZE;	//다음위치로 i조정

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{	//front와 element들의 위치 출력
		if(i == cQ->front) {
			printf("  [%d] = front\n", i);	
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);

	}
	//printQ(cQ);
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);  //front와 rear의 위치 출력
}