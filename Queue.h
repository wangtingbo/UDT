/*
*	作者:龚高杰
*   邮箱:ggj2lian@163.com
*   时间:2016-05-26
*   地点:中南林业科技大学
**/

#ifndef     _QUEUE_H_
#define    _QUEUE_H_

#include <iostream>
using namespace std;

#define QUEUE_SIZE        5
#define WINDOW            20   //窗口大小

template <class T>
class Queue;

template <class T>
class info_node
{
	friend class Queue<T>;
public:
	info_node()
	{
		memset(&data,0,sizeof(data));
		next = NULL;
		pre  = NULL;
	}
private:
	T data;
	info_node<T>* next;
	info_node<T>* pre;
};

template <class T>
class Queue
{
public:
	Queue() 
	{
		size = QUEUE_SIZE;
		really_len = 0;
		head = NULL;
		last = NULL;
		Create_Queue();
	}
/**************************************************************
*	函数:int Create_Queue() 
*	函数功能:创建队列
*	参数:没有参数
*	返回值:无返回值
***************************************************************/	
	void Create_Queue()                                 
	{
		int i = 0;
		for(i = 0;i<QUEUE_SIZE;i++)
		{
			info_node<T>* tmp = new info_node<T>;
			while(tmp == NULL)
			{
				tmp = new info_node<T>;
			}
			if(head == NULL)
			{
				head = tmp;
				last  = head;
			}
			else
			{
				last->next = tmp;
				tmp->pre = last;
				last = tmp;
			}
		}
		printf("create queue ok!\n");
	}

/*********************************************************
*	函数:push_queue(T data)  
*	函数功能:将节点信息放入到队列中去
*	返回值:返回1,表示该队列已经达到最长度的大小限制,返回0表示该队列还有剩余的空间
*	参数:data为节点信息
**********************************************************/	
	int push_queue(T data,int fg)                             
	{
		really_len++;
		if(really_len > size -1 && fg == 1)
		{
			add_queue_length();
		}
		T tmpdata;
		memset(&tmpdata,0,sizeof(tmpdata));
		info_node<T>* tmp = head;
		if(memcmp(&tmp->data,&tmpdata,sizeof(tmpdata))==0)
		{
			head->data = data;
		}
		else
		{
			while(memcmp(&tmp->data,&tmpdata,sizeof(tmpdata))!=0)
			{
				tmp = tmp->next;
			}
			if(tmp != NULL)
			{
				tmp->data = data;
				return 0;
			}
			else
			{
				return 1;  
			}
		}
		
	}

/*********************************************************
*	函数:push_suitable_pos 
*	函数功能:将节点信息放入到队列中合适的地方去
*	返回值:无返回值
*	参数:data为节点信息
**********************************************************/	
	void push_suitable_pos(T data)                             
	{
		info_node<T>* tmp = head;
		while(tmp!=NULL)
		{
			if(tmp->data.pack.head.Pack_id == 0)   //说明已经到可以使用的地方
			//if(tmp->data == 0)
			{
				printf("Queue::132行\n");
				tmp->data = data;
				really_len++;
				break;
			}
			tmp = tmp->next;
		}
	}

/******************************************************************
*	函数:T Get_val(int num=0) 
*	函数功能:获得这个队列的指定的元素
*	参数:num表示要获得哪个元素的地址
*	返回值:返回找到的这个元素
*******************************************************************/
	T   Get_val(int num=0)                            
	{
		T data;
		memset(&data,0,sizeof(data));

		int i = 0;
		info_node<T>* tmp = head;
		for(i = 0;i < num && tmp!=NULL;i++)
		{
			tmp = tmp->next;
		}
		
		if(tmp != NULL)
		{
			return tmp->data;
		}
		else
		{
			return data;
		}
	}

/******************************************************************
*	函数:get_val(int num=0) 
*	函数功能:得到这个队列的指定元素,并将读取过一次的元素放到最后一个不为空节点的后面
*	参数:num表示要获得哪个元素的地址
*	返回值:返回指向给节点的指针,以便于修改该节点的内部的值
*******************************************************************/
	T*   get_val(int num=0)                            
	{
		T data;
		memset(&data,0,sizeof(data));
		int i = 0;
		info_node<T>* tmp = head;
		for(i = 0;i < num && tmp!=NULL;i++)
		{
			tmp = tmp->next;
		}
		if(tmp != NULL)
		{
			move_node();
			return (T*)(&tmp->data);
		}
		else
		{
			return NULL;
		}
	}

/****************************************************************************
*	函数:Copy_All_Node
*	功能:将指定队列里面的节点值,拷贝到当前队列中
*	参数:q表示原队列
*	返回值:没有返回值
*****************************************************************************/
	void Copy_All_Node(Queue<T> *q)
	{
		info_node<T>* q_head = q->get_head();
		int length = q->get_reallysize();
		
		info_node<T>* tmp_node = head;
		int i = 0;
		while(i<length)
		{
			if(really_len < size)
			{
				tmp_node->data = q_head->data;
				tmp_node = tmp_node->next;
				q_head = q_head->next;
				really_len++;
			}	
			else
			{
				add_queue_length();    //默认增加10个长度
				tmp_node->data = q_head->data;
				tmp_node = tmp_node->next;
				q_head = q_head->next;
				really_len++;
			}
			i++;
		}
	}

/****************************************************************************
*	函数:delete_node
*	功能:删除指定队列,指定位置内的值,并将这个节点移动到该队列的最后面
*	参数:pos表示要删除的节点在队列中的位置,q表示要要从这个队列里面删除节点
*	返回值:返回-1为失败,返回0为成功
*****************************************************************************/
	int delete_node(int pos,Queue& q)                                
	{
		if(pos > really_len)
		{
			return -1;
		}
		else
		{
			info_node<T>* tmp = q.get_head();
			int  i = 0;
			while(i<pos && tmp!=NULL)
			{
				tmp = tmp->next;
				i++;
			}
			//printf("删除的节点为 %d \n",tmp->data.sockfd);
			if(tmp!=NULL)
			{
				if(tmp->next == NULL)
				{					
					memset(&(tmp->data),0,sizeof(tmp->data));
					//printf("情况1    %d\n",tmp->data.sockfd);
					q.sub_really_len();
					return 0;
				}
				else if(tmp->next!=NULL && tmp->pre!=NULL)
				{
					info_node<T>* tmp2 = tmp->pre;
					tmp2->next = tmp->next;
					last->next = tmp;
					tmp->pre = last;
					tmp->next = NULL;
					last = tmp;
					memset(&(tmp->data),0,sizeof(tmp->data));
					//printf("情况2    %d\n",tmp->data.sockfd);
					q.sub_really_len();
					return 0;
				}
				else if(tmp->pre == NULL)
				{
					memset(&(tmp->data),0,sizeof(tmp->data));
					//printf("情况3    %d\n",tmp->data.sockfd);
					q.sub_really_len();
					return 0;
				}
			}
			else
			{
				return -1;
			}
		}
	}

/****************************************************************************
*	函数:delte_all_node
*	功能:删除指定队列内部所有的值
*	参数:q表示这个队列
*	返回值:返回-1表示失败,返回0表示成功
*****************************************************************************/
	int delete_all_node(Queue<T> *q)
	{
		info_node<T>* tmp =  q->get_head();
		info_node<T>* tmp2 = tmp;
		while(tmp!=NULL)
		{
			memset(&(tmp->data),0,sizeof(info_node<T>));   //将所有的节点内部值清零
			tmp = tmp->next;
		}
		q->set_really_len(0);
		if(q->sub_queue_length(q->get_size()-20) >= 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

/****************************************************************************
*	函数:delte_all_node
*	功能:删除队列内部所有的值
*	参数:无参
*	返回值:返回-1表示失败,返回0表示成功
*****************************************************************************/
	int delete_all_node()
	{
		info_node<T>* tmp =  head;
		info_node<T>* tmp2 = tmp;
		while(tmp!=NULL)
		{
			memset(&(tmp->data),0,sizeof(tmp->data));   //将所有的节点内部值清零
			tmp = tmp->next;
		}
		really_len = 0;
		if(sub_queue_length(get_size()-20) >= 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

/****************************************************************************
*	函数:delete_node
*	功能:删除一个编号的节点里面的data,并将这个节点移动到最后面
*	参数:num表示删除的是哪一个节点的值
*	返回值:返回-1为失败,返回0为成功
*	问题:这里我先使用轮询的方法去查找所需要的数据,可以将其改进为使用红黑树,即map
*容器进行存储.
*****************************************************************************/
	int delete_node(int pack_id)                                
	{
		//因为pack_id不为0的时候节点都是在前面,所以我从头开始索引
		int i = 0;
		info_node<T>* tmp = head;
		while(i<really_len && tmp!=NULL)
		{
			if(tmp->data.pack.head.Pack_id == pack_id)    //这里需要修改,在真实使用的时候
			 //if(tmp->data == pack_id)
			{
	
				if(tmp == head)
				{
					memset(&(tmp->data),0,sizeof(tmp->data));
	
					head = head->next;
					tmp->next = NULL;
					last->next = tmp;
					tmp->pre = last;
					last = tmp;
					really_len--;
	
					return 0;
				}
				else
				{

					memset(&(tmp->data),0,sizeof(tmp->data));
					info_node<T>* pre_node = tmp->pre;
				
					info_node<T>* next_node = tmp->next;
					
					pre_node->next = next_node;
						
					next_node->pre = pre_node;
	
					last->next = tmp;
					tmp->pre = last;
					tmp->next = NULL;
					last = tmp;
					really_len--;
	
					return 0;
				}
			}
			tmp = tmp->next;
			i++;
		}
		return -1;
	}

/*****************************************************************
*	函数:exchange
*	函数功能:将删除的节点与最后一个位置(有效值)的交换
*	参数:num表示增加的长度
*	返回值:成功的时候返回10
******************************************************************/
	int exchange(int pack_id)
	{
		//先找到该节点对应的节点
		info_node<T>* tmp = head;
		while(tmp!=NULL)
		{
			if(tmp->data.pack.head.Pack_id == pack_id)
			//if(tmp->data == pack_id)
			{
				printf("更换的节点是 = %d\n",pack_id);
				//找到节点之后,需要判断最后一个节点是否是有效的
				//如果最后一个节点没有有效值的话,将这个值放在最后,在该值的后面的值向前移动
				if(tmp==head)    //如果是头结点
				{
					head = head->next;
					head->pre = NULL;
					last->next = tmp;
					tmp->pre = last;
					tmp->next = NULL;
					last = tmp;
					memset(&last->data,0,sizeof(last->data));   //将其变成0
				}
				else if(tmp == last)   //如果该节点为尾节点
				{
					memset(&last->data,0,sizeof(last->data)); 
					//nothing to do
				}	
				else                   //如果是中间
				{
					//如果是第二个节点需要被移位
					info_node<T>* front = tmp->pre;       //头结点 
					info_node<T>* nextnode = tmp->next;   //3号
					front->next = nextnode;
					nextnode->pre = front;

					last->next = tmp;
					tmp->pre = last;
					tmp->next = NULL;
					last = tmp;
					memset(&last->data,0,sizeof(last->data));					
				}
				really_len--;
				return 0;
			}
			tmp = tmp->next;
		}
		return -1;
	}

/*****************************************************************
*	函数:       add_queue_length
*	函数功能:   增加队列的长度
*	参数:       num表示增加的长度
*	返回值:     成功的时候返回10
******************************************************************/
	int add_queue_length(int num=10)	                 //动态增加队列的长度
	{
		printf("增加10个长度\n");
		info_node<T>* header = new info_node<T>;
		info_node<T>* temp   = header;
		int i = 0;
		for(i=0;i<num;i++)
		{
			info_node<T>* newnode = new info_node<T>;
			temp->next = newnode;
			newnode->pre = temp;
			temp = newnode; 
		}
		last->next = header;
		header->pre = last;
		last = temp;
		size = size + 10;
		return 10;
	}

/*****************************************************************
*	函数: 			sub_queue_length
*	函数功能:       减少队列的长度
*	参数:           num表示减少的长度
*	返回值:         返回删除的长度
******************************************************************/	
	int sub_queue_length(int num)                        
	{
		if(num >= 0)
		{
			printf("减少队列 %d 的长度\n",num);
			int i = 0;
			info_node<T>* tmp = last;
			info_node<T>* tmp2 = tmp;
			while(i<num)
			{
				tmp2 = tmp->pre;
				delete tmp;
				tmp = tmp2;
				i++;
			}
			last = tmp2;
			last->next = NULL;
			
			int unable_length = size - really_len;  //算出没有使用的空间
			if(num > unable_length)    //如果要删除的长度比没有使用的空间长度大的时候           
  			{
  				really_len = really_len - (num - unable_length);
			}
			size = size - num;
			return num;
		}
		else
		{
			return -1;
		}
	}

/*****************************************************************
*	函数:get_size
*	函数功能:得到该队列最大的可使用长度
*	参数:无参
*	返回值:返回队列长度
******************************************************************/	
	int get_size()
	{
		return size;
	}                                                          
	int get_reallysize()
	{
		return really_len;
	}
	info_node<T>* get_head()
	{
		return head;
	}
	info_node<T>* get_last()
	{
		return last;
	}

/*****************************************************************
*	函数:showdata
*	函数功能:显示所有的数据
*	参数:无参
*	返回值:无返回值
******************************************************************/		
	void showdata()
	{
		info_node<T>* tmp = head;
		int i = 0;
		for(i = 0; i <really_len;i++)
		{
			printf("%d\t",tmp->data.pack.head.Pack_id);
			//printf("%d\t",tmp->data);
			tmp = tmp->next;
		}
		printf("\n");
	}

	void reshowdata()
	{
		info_node<T>* tmp = last;
		int i = 0;
		for(i = 0; i <= size;i++)
		{
			printf("%d\t",tmp->data.head.Pack_id);
			tmp = tmp->pre;
		}
		printf("\n");
	}

/*****************************************************************
*	函数:move_node
*	函数功能:将头结点移动到最后有效的节点后面
*	参数:无参
*	无返回值
******************************************************************/
	void move_node()
	{
		int i = 0;
		info_node<T>* tmp = head;
		
		//然后再找到移动后需要放置的位置
		//然后将该节点取出来放置到那个位置,然后该节点后面的节点,向前移动一位
		//printf("really_len = %d\n",really_len);
		if(really_len == size)
		{
			last->next = tmp;
			tmp->pre   = last;
			last = tmp;
			head = head->next;
			head->pre = NULL;
			tmp->next = NULL; 
		}
		else if(really_len == 1)
		{
			//nothing to do
		}
		else
		{
			info_node<T>* tmp2 = head;
			info_node<T>* tmp3 = NULL;
			while(i < really_len-1)
			{
				tmp2 = tmp2->next;
				i++;
			}
			head = head->next;
			head->pre = NULL;
			tmp->pre = tmp2;	  
			tmp3 = tmp2->next;    //tmp3是tmp2的写一个节点
			tmp2->next = tmp;
			tmp->next = tmp3;
		}
	}

/*********************************************************
*	函数:sub_really_len()
*	函数功能:减少真正的在使用的长度
*	返回值:无返回值
*	参数:没有参数
**********************************************************/
	void sub_really_len()
	{
		really_len--;
	}

/*********************************************************
*	函数:add_really_len()
*	函数功能:增加真正的在使用的长度
*	返回值:无返回值
*	参数:没有参数
**********************************************************/
	void add_really_len()
	{
		really_len++;
	}


	void set_really_len(int _really_len)
	{
		really_len = _really_len;
	}
/*********************************************************
*	函数:~Queue()
*	函数功能:析构函数,释放所有的节点
*	返回值:无返回值
*	参数:没有参数
**********************************************************/
	~Queue()
	{
		delete_queue_node();
	}

private:
/*********************************************************
*	函数:delete_queue_node() 
*	函数功能:释放所有的节点,提供给析构函数使用,或者可以手动使用(不建议手动使用)
*	返回值:无返回值
*	参数:没有参数
**********************************************************/
	void delete_queue_node()                             //删除队列里面的节点
	{
		info_node<T>* tmp = head;
		while(head!=NULL)
		{
			head = head->next;
			delete tmp;
			tmp = head;
		}
	}
private:
	int size;                //该队列最大的长度,默认值为20
	int really_len;          //该队列中真实使用的长度
	info_node<T>* head;      //头指针
	info_node<T>* last;      //尾指针

};
#endif
