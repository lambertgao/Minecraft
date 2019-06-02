#include <stdio.h>
#include <math.h>

#define Maxsize 100   //the upper using limit of Ender Eyes are 100 times
#define Inter_Max 5050   //n*(n+1)/2, n stands for how many Ender Eyes are used

double pi=3.1415926;

/* bool */
typedef int bool;
#define true 1
#define false 0

/* List of All Threw Points */
typedef struct{
	double x;
	double z;
	double c;   //the variable in F3
}ThrewPoint;   //points with xzc
typedef struct{
	ThrewPoint points[Maxsize];
	int len;
}ThrewList;

/* List of All Points of Intersection */ 
typedef struct{
	double x;
	double z;
}Inter;   //points with xz
typedef struct{
	Inter points[Inter_Max];
	int len;
}InterList;

/* Circle */
typedef struct{
	Inter o;   //center location
	double r;   //radius
}Circle;

/* Line */
typedef struct{
	double A;
	double B;
	double C;
}Line;   //Ax+By=C stands for a line

/* Get all the points of intersection and store them into a list */
Inter Calc_Inter(ThrewPoint a,ThrewPoint b){
	Inter m;
	m.x=(b.x*sin(a.c*pi/180)*cos(b.c*pi/180)-a.x*cos(a.c*pi/180)*sin(b.c*pi/180)-(a.z-b.z)*sin(a.c*pi/180)*sin(b.c*pi/180))/sin(a.c*pi/180-b.c*pi/180);
	m.z=(a.z*sin(a.c*pi/180)*cos(b.c*pi/180)-b.z*cos(a.c*pi/180)*sin(b.c*pi/180)+(a.x-b.x)*cos(a.c*pi/180)*cos(b.c*pi/180))/sin(a.c*pi/180-b.c*pi/180);
	return m;
}
void Collect_Inter(ThrewList L,InterList *S){
	int i,j=0;
	S->len=0;
	for(i=0;i<L.len;i++)
		for(j=i+1;j<L.len;j++){
			if(L.points[i].c!=L.points[j].c){
				S->points[S->len]=Calc_Inter(L.points[i],L.points[j]);
				S->len++;
			}
		}
}

/* Get the minimal circle which covers all the points of intersections */
double Distance(Inter a,Inter b){
	return sqrt(pow(a.x-b.x,2)+pow(a.z-b.z,2));
}
bool In_Circle(Inter a,Circle q){
	if(Distance(a,q.o)<=q.r)
		return true;
	else
		return false;
}
Line Mid_Vertical(Inter a,Inter b){
	Line l;
	l.A=b.x-a.x;
	l.B=b.z-a.z;
	l.C=((pow(b.x,2)-pow(a.x,2))+(pow(b.z,2)-pow(a.z,2)))/2;
	return l;
}
Inter Cross(Line l1,Line l2){
	Inter p;
	double delta;
	delta=l1.A*l2.B-l2.A*l1.B;
	if(delta!=0){
		p.x=(l2.B*l1.C-l1.B*l2.C)/delta;
		p.z=(l1.A*l2.C-l2.A*l1.C)/delta;
	}
	return p;
}
bool Is_Parallel(Line l1,Line l2){
	double delta;
	delta=l1.A*l2.B-l2.A*l1.B;
	if(delta==0)
		return true;
	else
		return false;
}
Circle MinimalCircle(InterList S){
	Circle Q;
	int i,j,k;
	Line L1,L2;
	for(i=0;i<S.len;i++)
		if(!In_Circle(S.points[i],Q)){
			Q.o=S.points[i];
			Q.r=0;
			for(j=0;j<i;j++)
				if(!In_Circle(S.points[j],Q)){
					(Q.o).x=((S.points[i]).x+(S.points[j]).x)/2;
					(Q.o).z=((S.points[i]).z+(S.points[j]).z)/2;
					Q.r=Distance(S.points[i],Q.o);
					for(k=0;k<j;k++)
						if(!In_Circle(S.points[k],Q)){
							L1=Mid_Vertical(S.points[i],S.points[j]);
							L2=Mid_Vertical(S.points[j],S.points[k]);
							if(!Is_Parallel(L1,L2))
								Q.o=Cross(L1,L2);
							Q.r=Distance(S.points[k],Q.o);
						}
				}
		}
	return Q;
}

/* main */
int main(){
	int i,j,n;
	ThrewList T;
	InterList I;
	Circle C;
	T.len=0;
	I.len=0;
	printf("Please input the times of using Ender Eyes:\n");
	scanf("%d",&n);
	printf("\n");
	if(n<2)
		printf("error: Please use Ender Eyes at least twice!\n\n");
	else if(n>100)
		printf("error: Too much times, please no more than 100!\n\n");
	else{
		for(i=1,j=0;i<=n;i++,j++){
			printf("No.%d x=",i);
			scanf("%lf",&(T.points[j]).x);
			printf("     z=");
			scanf("%lf",&(T.points[j]).z);
			printf("     c=");
			scanf("%lf",&(T.points[j]).c);
			T.len++;
		}
		Collect_Inter(T,&I);
		printf("\nAll possible coordinates (x,z):\n");
		for(i=0;i<I.len;i++)
			printf("(%.1f,%.1f) ",(I.points[i]).x,(I.points[i]).z);
		C=MinimalCircle(I);
		printf("\n\nAll coordinates locate in a circle:\n");
		printf("center(x,z): (%.1f,%.1f), r=%.0f\n\n",(C.o).x,(C.o).z,C.r);
	}
	system("pause");
	return 0;
}