#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*********************************************************************
 Modified from 
ALTERNATING BIT AND GO-BACK-N NETWORK EMULATOR: VERSION 1.1  J.F.Kurose
inter-datagram time: uniformly random in [0, 2*lambda] (e.g., lambda = 20)
propagation time from A to B: uniformly random in [1, 10]
You may determine the time-out value appropriately (e.g., 15)
**********************************************************************/

#define BIDIRECTIONAL 0    /*  Unidirectional */

/* a datagram "dgm" is the data unit passed from layer 3 (teachers code) to layer  */
/* 2 (students' code).  It contains the data (characters) to be delivered */
/* to layer 3 via the students data link layer protocol entities.         */
struct dgm {
  char data[20];
  };

/* a frame is the data unit passed from layer 2 (students code) to layer */
/* 1 (teachers code).  Note the pre-defined frame structure, which all   */
/* students must follow. */
struct frm {
   int seqnum;
   int acknum;
   int checksum;
   char payload[20];
   };

struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct frm *frmptr;     /* ptr to frame (if any) assoc w/ this event */
   struct event *prev;     /* previous event pointer */
   struct event *next;     /* next event pointer */
 };
struct event *evlist = NULL;   /* the event list */

void starttimer(int time, float increment);
void bufferupdate(struct frm frame);
int checksum(int mod, int checksum, char data[]);
int A_input(struct frm frame);
void A_output(struct dgm datagram);
void B_input(struct frm frame);
void B_output(struct dgm datagram);
void A_timerinterrupt();
void init();
float jimsrand();
void generate_next_arrival();
void insertevent(struct event *p);
void printevlist();
void stoptimer(int AorB);
void tolayer1(int AorB,struct frm frame);
void tolayer3(int AorB,char datasent[20]);

float delay = 0.000;            // for Average Delay calculation
float time_sent = 0.000;        // for Average Delay calculation
float time_rcv = 0.000;         // for Average Delay calculation
float delay_total = 0.000;      // for Average Delay calculation
float avg_delay = 0.000;        // for Average Delay calculation
int num_rcvfrm = 0;             // for Average Delay calculation

/* possible events: */
#define  TIMER_INTERRUPT   0  
#define  FROM_LAYER3       1
#define  FROM_LAYER1       2
#define  TIMEOUT           30.0
#define  OFF               0
#define  ON                1
#define  A                 0
#define  B                 1

// mydefinelist
#define MAKE 0
#define CHECK 1
#define RIGHT(x) ((x)&(0xffff))
#define LEFT(x) (((x)&(0xffff0000))>>16)


int TRACE = 0;             /* for my debugging */
int nsim = 0;              /* number of datagrams from 3 to 2 so far */ 
int nsimmax = 0;           /* number of max datagrams to generate, then stop */
float time = 0.000;
float lossprob;            /* probability that a frame is dropped  */
float corruptprob;         /* probability that one bit in frame is flipped */
float lambda;              /* arrival rate of datagrams from layer 3 */   
int   ntolayer1;           /* number of frames sent into layer 1 */
int   nlost;               /* number of frames lost in media */
int   ncorrupt;            /* number of frames corrupted by media */

struct frm buffer;
int nextseqnum = 1;
int readyseq = 1;
int totalfrm = 0;
/********* STUDENTS WRITE THE NEXT ROUTINES *********/


/*========== A_output ===========*/
/* called from layer 3, pass the datagram to be sent to other side */

void bufferupdate(struct frm frame){
   buffer.checksum = frame.checksum;
   buffer.acknum = frame.acknum;
   buffer.seqnum = frame.seqnum;
   for(int i=0;i<20;i++){
      buffer.payload[i] = frame.payload[i];
   }
}
int checksum(int mod, int checksum, char data[20]){
   int i;
   int sum = 0;
   int left, right;
   int newsum;
   for(i=0;i<20;i++){
      sum += data[i];
   }
   if(mod == CHECK){
      sum += checksum;
   }
   newsum = ~(LEFT(sum) + RIGHT(sum));

   if(mod == MAKE){
      return newsum;
   }
   if(mod == CHECK){
      if(sum == -1){
         return 1;
      }
      else{
         return -1;
      }
   }
}

void A_output(struct dgm datagram)
{
   struct frm frame;

   frame.checksum = checksum(MAKE, 0, datagram.data);
   for(int i=0;i<20;i++){
         frame.payload[i] = datagram.data[i];
   }
   frame.seqnum = nextseqnum;
   frame.acknum = 0;

   bufferupdate(frame);

   printf("A: send %d seq\n", frame.seqnum);
   tolayer1(A, frame);
   starttimer(A, TIMEOUT);

   nextseqnum++;
}


/*========== A_input ===========*/
/* called from layer 1, when a frame arrives for layer 2 */
int A_input(struct frm frame)
{
   if(frame.acknum != 0) {
      if(frame.acknum != readyseq){
         printf("A: Bad acknum: ready for %d, accept %d\n", readyseq, frame.acknum);
         tolayer1(A,buffer);
         starttimer(A,TIMEOUT);
         return -1;
      }
   }else if(checksum(CHECK, frame.checksum, frame.payload) < 0){
      printf("A: Bad checksum\n");
      return -1;
   }
   //printf("pass the check routine : checksum\n");
   if(frame.acknum == 0){
      struct frm ack;
      ack.seqnum = frame.seqnum;
      ack.acknum = frame.seqnum;
      for(int i = 0; i< 20; i++){
         ack.payload[i] = frame.payload[i];
      }
      ack.checksum = checksum(MAKE, 0, ack.payload);
      tolayer1(A,ack);
      tolayer3(A, frame.payload);
   }
   else{
      printf("A: recv %d seq\n", frame.seqnum);
      printf("one frame is process successfully: %d seq\n", frame.seqnum);
      if(nsim >= nsimmax){
         if(TRACE > 2)
            printevlist();
         return 1;
      }
      stoptimer(A);
      readyseq++;
      generate_next_arrival();
   }
   return 0;
}


/*========== A_timerinterrupt ===========*/
/* called when A's timer goes off */
void A_timerinterrupt()
{
   //A_output(frame.payload);
   //stoptimer(A);

   tolayer1(A,buffer);
   printf("A: TIMEOUT, send packet again, %d seq\n", buffer.seqnum);
   evlist->evtype = -1;
   starttimer(A,TIMEOUT);
}

/*========== A_init ===========*/
/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
// A_init()
// {

// }


/*========== B_output (Do not need)===========*/
/* Note that with simplex transfer from A-to-B, there is no B_output() */
/* don't need for unidirectional communication */
void B_output(struct dgm datagram)
{
   struct frm frame;
   frame.checksum = checksum(MAKE, 0, datagram.data);
   for(int i=0;i<20;i++){
      frame.payload[i] = datagram.data[i];
   }
   frame.seqnum = nextseqnum;
   frame.acknum = 0;
   tolayer1(B, frame);
}



/*========== B_input ===========*/
/* called from layer 1, when a frame arrives for layer 2 at B*/
void B_input(struct frm frame)
{
   if(checksum(CHECK, frame.checksum, frame.payload) < 0){
      printf("B: Bad checksum\n");
      return;
   }

   if(frame.acknum == 0){
      struct frm ack;
      ack.seqnum = frame.seqnum;
      ack.acknum = frame.seqnum;
      for(int i=0;i<20;i++){
         ack.payload[i] = frame.payload[i];
      }
      ack.checksum = checksum(MAKE, 0, ack.payload);
      printf("B: send %d ack\n", ack.acknum);
      tolayer1(B,ack);
   }
   else{
      tolayer3(B,frame.payload);
   }
   return;
}


/*========== B_timerinterrupt (Do not need)===========*/
/* called when B's timer goes off */
// B_timerinterrupt() /* don't need for unidirectional communication */

// {

// }



/*========== B_init ===========*/
/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
// B_init()
// {

// }
/*========== main ===========*/
/*****************************************************************
***************** NETWORK EMULATION CODE STARTS BELOW ***********
The code below emulates the layer 1 and below network environment:
  - emulates the transmission and delivery (possibly with bit-level corruption
    and frame loss) of frames across the layer 1/2 interface
  - handles the starting/stopping of a timer, and generates timer
    interrupts (resulting in calling students timer handler).
  - generates datagram to be sent (passed from later 3 to 2)

YOU SHOLD NOT TOUCH, OR REFERENCE (in your code) ANY
OF THE DATA STRUCTURES BELOW.  If you're interested in how I designed
the emulator, you're welcome to look at the code - but again, 
and you definitely should not have to modify.
******************************************************************/


int main()
{
   struct event *eventptr;
   struct dgm  dgm2give;
   struct frm  frm2give;
   init();

   int i,j;
   char c;
   
   while (1) {
      eventptr = evlist;            /* get the next event to simulate from the event list */
      if (eventptr==NULL)
         goto terminate;
      if (TRACE>=2) {
         printf("\nEVENT time: %f,",eventptr->evtime);
         printf("  type: %d",eventptr->evtype);
         if (eventptr->evtype==0)
	         printf(", timerinterrupt  ");
         else if (eventptr->evtype==1)
            printf(", fromlayer3 ");
         else
	         printf(", fromlayer1 ");
         printf(" entity: %d\n",eventptr->eventity);
      if ((eventptr->evtype==1)&&(eventptr->eventity==0))   // for Average Delay calculation
         time_sent = eventptr->evtime;                      // for Average Delay calculation
      if ((eventptr->evtype==2)&&(eventptr->eventity==1)){  // for Average Delay calculation
         time_rcv = eventptr->evtime;                       // for Average Delay calculation
         delay = time_rcv - time_sent;                      // for Average Delay calculation
         num_rcvfrm++;                                      // for Average Delay calculation
         delay_total += delay;}                             // for Average Delay calculation
      }
      time = eventptr->evtime;        /* update time to the current event time */
      if (eventptr->evtype == FROM_LAYER3 ) {
         if (nsim==nsimmax)
	         break;                        /* all done with simulation */
         /* set up future datagram arrival */
         /* fill in datagram to give with string of the same letter */    
         j = nsim % 26; 
         for (i=0; i<20; i++)  
            dgm2give.data[i] = 97 + j;
         if (TRACE>2) {
            printf("          MAINLOOP: data given to student: ");
            for (i=0; i<20; i++) 
               printf("%c", dgm2give.data[i]);
            printf("\n");
	      }
         nsim++;
         if (eventptr->eventity == A) {
            A_output(dgm2give);  
         }
         evlist = evlist->next;        /* remove the event from the event list */
      }
      else if (eventptr->evtype ==  FROM_LAYER1) {
         frm2give.seqnum = eventptr->frmptr->seqnum;
         frm2give.acknum = eventptr->frmptr->acknum;
         frm2give.checksum = eventptr->frmptr->checksum;
         for (i=0; i<20; i++)  
            frm2give.payload[i] = eventptr->frmptr->payload[i];
         if (TRACE>2) {
            printf("          MAINLOOP: data given to student: ");
            for(i=0;i<20;i++)
               printf("%c", frm2give.payload[i]);
            printf(", seq:%d, ack:%d\n",frm2give.seqnum,frm2give.acknum);
         }
	      if (eventptr->eventity ==A) {     /* deliver frame by calling */
   	      if(A_input(frm2give) == 1)
               goto terminate;             /* appropriate entity */
         } else {
   	      B_input(frm2give);
	      }
	      free(eventptr->frmptr);          /* free the memory for frame */
         evlist = evlist->next;        /* remove the event from the event list */
      }
      else if (eventptr->evtype ==  TIMER_INTERRUPT) {
         if (eventptr->eventity == A) {
	         A_timerinterrupt();
            evlist = evlist->next;
            evlist->prev = NULL;
         }
         else  {
	         printf("INTERNAL PANIC: unknown event type \n");
         }
         free(eventptr);
      }
      //printevlist();
      if (evlist!=NULL)
         evlist->prev=NULL;
      else
         goto terminate;
   }

terminate:
   printf(" Simulator terminated at time %f\n total %d frame is sended, %d is lost, %d is corrupted\ntotal seqnum:%d\n",time,ntolayer1,nlost, ncorrupt, nextseqnum-1);
    if ( TRACE>2){                              // for Average Delay calculation
   avg_delay = delay_total/num_rcvfrm;          // for Average Delay calculation
   printf("\nAverage Delay: %f\n", avg_delay);} // for Average Delay calculation
}
/*========== main ===========*/

/*========== init ===========*/
void init()                         /* initialize the simulator */
{
  int i;
  float sum, avg;
  float jimsrand();
  
  
   printf("-----  Stop and Wait Network Simulator Version 1.1 -------- \n\n");
   printf("Enter the number of datagrams to simulate: ");
   scanf("%d",&nsimmax);
   printf("Enter the frame loss probability [0.0 for no loss]:");
   scanf("%f",&lossprob);
   printf("Enter the frame corruption probability [0.0 for no corruption]:");
   scanf("%f",&corruptprob);
   printf("Enter the average time between datagrams from sender's layer3 [ > 0.0]:");
   scanf("%f",&lambda);
   printf("Enter TRACE:");
   scanf("%d",&TRACE);

   
   srand(9999);              /* init random number generator */
   sum = 0.0;                /* test random number generator for students */
   for (i=0; i<1000; i++)
      sum=sum+jimsrand();    /* jimsrand() should be uniform in [0,1] */
   avg = sum/1000.0;
   if (avg < 0.25 || avg > 0.75) {
    printf("It is likely that random number generation on your machine\n" ); 
    printf("is different from what this emulator expects.  Please take\n");
    printf("a look at the routine jimsrand() in the emulator code. Sorry. \n");
    exit(0);
    }

   ntolayer1 = 0;
   nlost = 0;
   ncorrupt = 0;

   time=0.0;                    /* initialize time to 0.0 */
   generate_next_arrival();     /* initialize the event list */
}
/*========== init ===========*/

/*========== jimsrand ===========*/
/****************************************************************************/
/* jimsrand(): returns a float in range [0,1].  The routine below is used to */
/* isolate all random number generation in one location.  We assume that the*/
/* system-supplied rand() function return an int in the range [0,mmm]        */
/****************************************************************************/
float jimsrand() 
{
  double mmm = 2147483647;   /* largest int  - MACHINE DEPENDENT!!!!!!!!   */
  float x;                   /* individual students may need to change mmm */ 
  x = rand()/mmm;            /* x should be uniform in [0,1] */
  return(x);
}  
/*========== jimsrand ===========*/

/************** EVENT HANDLINE ROUTINES **************/
/*  The next set of routines handle the event list   */
/*****************************************************/
/*========== generate_next_arrival ===========*/
void generate_next_arrival()
{
   double x,log(),ceil();
   struct event *evptr;
   float ttime;
   int tempint;
   if(nsim >= nsimmax)
      return;
   if (TRACE>2)
       printf("          GENERATE NEXT ARRIVAL: creating new arrival\n");
 
   x = lambda*jimsrand()*2;  /* x is uniform on [0,2*lambda] */
                             /* having mean of lambda        */
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + x;
   evptr->evtype =  FROM_LAYER3;
   if (BIDIRECTIONAL && (jimsrand()>0.5) ) /* randomly create datagram arrival */
      evptr->eventity = B;
    else
      evptr->eventity = A;
   insertevent(evptr);
} 
/*========== generate_next_arrival ===========*/

/*========== insertevent ===========*/
void insertevent(struct event *p)
{
   struct event *q,*qold;

   if (TRACE>2) {
      printf("            INSERTEVENT: time is %lf\n",time);
      printf("            INSERTEVENT: type: %d, future time will be %lf\n",p->evtype, p->evtime); 
      }
   q = evlist;     /* q points to header of list in which p struct inserted */
   if (q==NULL) {   
      evlist=p;
      p->next=NULL;
      p->prev=NULL;
   }
   else {
      for (qold = q; q !=NULL && p->evtime > q->evtime; q=q->next)
            qold=q; 
      
      if (q==NULL) {
         qold->next = p;
         p->prev = qold;
         p->next = NULL;
      }
      else if (q==evlist) {
         p->next=evlist;
         p->prev=NULL;
         p->next->prev=p;
         evlist = p;
      }
      else {
         p->next = q;
         p->prev = q->prev;
         q->prev->next = p;
         q->prev = p;
      }
   }
}
/*========== insertevent ===========*/

/*========== printevlist ===========*/
void printevlist()
{
  struct event *q;
  int i;
  printf("--------------\nEvent List Follows:\n");
  for(q = evlist; q!=NULL; q=q->next) {
    printf("Event time: %f, type: %d entity: %d\n",q->evtime,q->evtype,q->eventity);
    }
  printf("--------------\n");
}
/*========== printevlist ===========*/

/****************** Student-callable ROUTINES **********************/
/* called by students routine to cancel a previously-started timer */
/*******************************************************************/
/*========== stoptimer ===========*/
void stoptimer(int AorB)
{
 struct event *q,*qold;

 if (TRACE>2)
    printf("          STOP TIMER: stopping timer at %f\n",time);
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
   for (q=evlist; q!=NULL ; q = q->next) {
      if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) { 
         /* remove this event */
         if (q->next==NULL && q->prev==NULL){
            evlist=NULL;         
         }
         else if (q->next==NULL){
            q->prev->next = NULL;
         }
         else if (q==evlist) { 
            q->next->prev=NULL;
            evlist = q->next;
         }
         else {    
            q->next->prev = q->prev;
            q->prev->next =  q->next;
         }
         free(q);
         return;
      }
   }
   printf("Warning: unable to cancel your timer. It wasn't running.\n");
}
/*========== stoptimer ===========*/

/*========== starttimer ===========*/
void starttimer(int AorB, float increment)
{

 struct event *q;
 struct event *evptr;

 if (TRACE>2)
    printf("          START TIMER: starting timer at %f, increment is %f\n",time, increment);
 /* be nice: check to see if timer is already started, if so, then  warn */
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next)  */
   for (q=evlist; q!=NULL ; q = q->next)  
    if ( (q->evtype==TIMER_INTERRUPT  && q->eventity==AorB) ) { 
      printf("Warning: attempt to start a timer that is already started\n");
      return;
      }
 
/* create future event for when timer goes off */
   evptr = (struct event *)malloc(sizeof(struct event));
   evptr->evtime =  time + increment;
   evptr->evtype =  TIMER_INTERRUPT;
   evptr->eventity = AorB;
   insertevent(evptr);
} 
/*========== starttimer ===========*/


/********** TOLAYER1 *************/
/*========== tolayer1 ===========*/
void tolayer1(int AorB,struct frm frame)
{
 struct frm *myfrmptr;
 struct event *evptr,*q;
 float lastime, x, jimsrand();
 int i;


 ntolayer1++;

 /* simulate losses: */
 if (jimsrand() < lossprob)  {
      nlost++;
      printf("PACKET LOST!\n");
      if (TRACE>0)    
	printf("          TOLAYER1: frame being lost\n");
      return;
    }  

/* make a copy of the frame student just gave me since he/she may decide */
/* to do something with the frame after we return back to him/her */ 
 myfrmptr = (struct frm *)malloc(sizeof(struct frm));
 myfrmptr->seqnum = frame.seqnum;
 myfrmptr->acknum = frame.acknum;
 myfrmptr->checksum = frame.checksum;
 for (i=0; i<20; i++)
    myfrmptr->payload[i] = frame.payload[i];
 if (TRACE>2)  {
   printf("          TOLAYER1: seq: %d, ack %d, check: %d ", myfrmptr->seqnum,
	  myfrmptr->acknum,  myfrmptr->checksum);
    for (i=0; i<20; i++)
        printf("%c",myfrmptr->payload[i]);
    printf("\n");
   }

/* create future event for arrival of frame at the other side */
  evptr = (struct event *)malloc(sizeof(struct event));
  evptr->evtype =  FROM_LAYER1;   /* frame will pop out from layer1 */
  evptr->eventity = (AorB+1) % 2; /* event occurs at the other entity */
  evptr->frmptr = myfrmptr;       /* save ptr to my copy of frame */
/* finally, compute the arrival time of frame at the other end.
   medium cannot reorder, so make sure frame arrives between 1 and 10
   time units after the latest arrival time of frames
   currently in the medium on their way to the destination */
 lastime = time;
/* for (q=evlist; q!=NULL && q->next!=NULL; q = q->next) */
 for (q=evlist; q!=NULL ; q = q->next) 
    if ( (q->evtype==FROM_LAYER1  && q->eventity==evptr->eventity) ) 
      lastime = q->evtime;
 evptr->evtime =  lastime + 1 + 9*jimsrand();

 /* simulate corruption: */
 if (jimsrand() < corruptprob)  {
    printf("CORRUPTION OCCUR!\n");
    ncorrupt++;
    if ( (x = jimsrand()) < .75)
       myfrmptr->payload[0]='Z';   /* corrupt payload */
      else if (x < .875)
       myfrmptr->seqnum = 999999;
      else
       myfrmptr->acknum = 999999;
    if (TRACE>0)    
	printf("          TOLAYER1: frame being corrupted\n");
    }  

  if (TRACE>2)  
     printf("          TOLAYER1: scheduling arrival on other side\n");
  insertevent(evptr);
} 
/*========== tolayer1 ===========*/

/*========== tolayer3 ===========*/
void tolayer3(int AorB,char datasent[20])
{
  int i;  
  if (TRACE>2) {
     printf("          TOLAYER3: data received: ");
     for (i=0; i<20; i++)  
        printf("%c",datasent[i]);
     printf("\n");
   }
  
}
/*========== tolayer3 ===========*/

