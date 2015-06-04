//use -lm to link sqrt function

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define Debug 0 //set this to 2 for a lot of numbers on output
#define Days 365
#define Excess (RAND_MAX/Days*Days)
int days[Days];
inline int rand_day()
{
	int n;
	while((n=rand())>=Excess);//rand() takes random number 
	return n/(Excess/Days);
}
//given p people,if n of them have same birthday in one run
int simulate1(int p,int n)
{
	//void *memset(void *s, int c, size_t n);
	//The  memset()  function  fills  the  first  n  bytes of the memory 		area pointed to by s with the constant byte c.

	memset(days,0,sizeof(days));
	while(p--)
		if(++days[rand_day()]==n)
			return 1;
	return 0;
}
//decide if probability of n out of np people sharing a birthday is above or below p_thresh,with n_sigmas sigmas confidence.note that if p_thresh is very low or high,minimum runs needs to be much higher
double prob(int np,int n,double n_sigmas,double p_thresh, double *std_dev)
{
	double p,d;//p:prob, d:standard deviation
	int runs=0,yes=0;
	do{
		yes+=simulate1(np,n);
		p=(double)yes/++runs;
		d=sqrt(p*(1-p)/runs);
		if(Debug>1)
			printf("\t\t%d:%d %d %g %g \r",np,yes,runs,p,d);
	}while(runs<10||fabs(p-p_thresh)<n_sigmas*d);
	if(Debug>1)
		putchar('\n');
	*std_dev=d;
	return p;
}
	
//bisect for truth
int find_half_chance(int n,double *p,double *dev)
{
	int low,high,mid;
reset:
	low=0;
	high=Days*(n-1)+1;
	do
	{
		mid=(high+low)/2;
		//5 sigma confidence . Conventionally people think 3 sigmas are good enough but for case of 5 people sharing birthday,3 sigmas actually sometimes give a slightly wrng answer
		*p=prob(mid,n,5,.5,dev);
		if(Debug)
			printf("\t%d%d%d%g%g\n",low,mid,high,*p,*dev);
		if(*p<.5)
			low=mid+1;
		else
			high=mid;
	
		if(high<low){

			//this happens when previous precisionswere too low;
			//easiest fix: reset
			if(Debug)
				puts("\tMade a mess, will redo");
			goto reset;
		}
	}while(low<mid || *p<0.5);
	return mid;
}

int main()
{

	int n,np;
	double p,d;
	// The srand() function sets its argument as the seed for a  new  		sequence of  pseudo-random  integers  to be returned by rand().  These 		sequences are repeatable by calling srand() with the same seed value.

	srand(time(0));
	for(n=2;n<=5;n++)
	{
		np=find_half_chance(n,&p,&d);
		printf(" %d people have birthday on same day out of %d people with probability p=%g +/- %g\n",n,np,p,d);
	}
	return 0;
}
	
//output
/*
shubham@ubuntu:~/Desktop$ ./a.out
 2 people have birthday on same day out of 23 people with probability p=0.507439 +/- 0.00148734
 3 people have birthday on same day out of 88 people with probability p=0.508736 +/- 0.00174682
 4 people have birthday on same day out of 187 people with probability p=0.502156 +/- 0.000431158
 5 people have birthday on same day out of 316 people with probability p=0.515965 +/- 0.00318924
*/
