#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <proto.h>

#include "thr_channel.h"
#include "server_conf.h"

struct thr_channel_ent_st
{
	chnid_t chnid;
	pthread_t tid;
};

struct thr_channel_ent_st thr_channel[CHNNR];
int tid_nextpos = 0;

static void *thr_channel_snder(void *ptr)
{
	struct msg_channel_st *sbufp;
	struct mlib_listentry_st *ent = ptr;
	int len;

	sbufp = malloc(MSG_CHANNEL_MAX);
	if(sbufp == NULL)
	{
		syslog(LOG_ERR,"malloc():%s",strerror(errno));
		exit(1);
	}
	sbufp->chnid = ent->chnid;

	while(1)
	{
		len = mlib_readchn(ent->chnid,sbufp->data,MAX_DATA);

		if(sendto(serversd,sbufp,len+sizeof(chnid_t),0,(void *)&sndaddr,sizeof(sndaddr)) < 0)
		{
			syslog(LOG_ERR,"thr_chnnel(%d):sendto():%s",ent->chnid,strerror(errno));
		}
		else
		{
			syslog(LOG_DEBUG,"thr_chnnel(%d):sento() successed",ent->chnid);
		}

		sched_yield();
	}
	pthread_exit(NULL);
}

int thr_channel_create(struct mlib_listentry_st *ptr)
{
	int err;

	err = pthread_create(&thr_channel[tid_nextpos].tid,NULL,thr_channel_snder,ptr);
	if(err)
	{
		syslog(LOG_WARNING,"pthread_create():%s",strerror(err));
		return -err;
	}
	
	thr_channel[tid_nextpos].chnid = ptr->chnid;
	tid_nextpos++;

	return 0;
}

int	thr_channel_destroy(struct mlib_listentry_st *ptr)
{
	int i;
	for(i = 0; i < CHNNR; i++)
	{
		if(thr_channel[i].chnid == ptr->chnid)
		{
			if(pthread_cancel(thr_channel[i].tid) < 0)
			{
				syslog(LOG_ERR,"pthread_cancel():the thread of channel %d",ptr->chnid);
				return -ESRCH;
			}
		}
		pthread_join(thr_channel[i].tid,NULL);
		thr_channel[i].chnid = -1;
		return 0;
	}
}
int thr_channel_destroyall(void)
{
	int i;
	for(i = 0; i < CHNNR; i++)
	{
		if(thr_channel[i].chnid > 0)
		{
			if(pthread_cancel(thr_channel[i].tid) < 0)
			{
				syslog(LOG_ERR,"pthread_cancel():the thread of channel %d",thr_channel[i].chnid);
				return -ESRCH;
			}
			pthread_join(thr_channel[i].tid,NULL);
			thr_channel[i].chnid = -1;
		}
	}
	return 0;

}















