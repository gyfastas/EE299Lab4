#ifdef QUEUE_H_
#define QUEUE_H_
//a easy queue class for note
#define MAX_LEN 16
class queue
{
  public:
  queue()
  { 
    //initialize it
    for (int i =0;i<MAX_LEN;i++)
    { 
      //dic = 0 ->no arrow
      data[i].dic = 0;
      data[i].isBeat = false;
    }
    front = rear = current_size  = 0;
  }
  bool empty()
  {
    if(front==rear)
    return 1;
    else
    return 0;
  }

  bool full()
  {
    if((rear+1)%MAX_LEN == front)
    {
      return 1;
    }
    else
      return 0;
  }

  bool pop()
  {
    if(empty())
    return 0;
    else
    { 
      front =(front+1)%MAX_LEN;
      current_size--;
      return 1;
    }
  }

  bool Front(note &x)
  {
    if(empty())
    {
      return 0;
    }
    else
    {
      x = data[front];
      return 1;
    }
  }

  bool Rear(note &x)
  {
    if(empty())
    {
      return 0;
    }
    else
    {
      x = data[rear];
      return 1;
    }
  }

  bool push(note x)
  {
    if((rear+1)%MAX_LEN==front)
      return 0;
    else
    { 
      rear = (rear+1)%MAX_LEN;
      data[rear].dic = x.dic;
      data[rear].isBeat = x.isBeat;
      current_size ++;
      return 1;
    }
  }

  bool single_generate()
  {
    if(full())
    {
      return 0;
    }
    else
    { 
      note x;
      x.dic = random(UP,LEFT+1);
      x.isBeat = false;
      push(x);
      return 1;
    }
  }
    //don't use it if you are not sure the size
  bool multi_generate(int num)
  { 
    note x;
    for(int i =0;i<num;i++)
    {
      x.dic = random(UP,LEFT+1);
      x.isBeat = false;
      push(x);
    }
  }
  int size()
  {
    return current_size;
  }

  private:
  note data[MAX_LEN];
  int current_size;
  int front;
  int rear;
};

#endif
