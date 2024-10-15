#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t));
  
  p -> root = nilNode;
  p -> nil = nilNode;
  nilNode -> color = RBTREE_BLACK; //RB Tree에서 nil 노드는 무조건 black
  nilNode -> left = NULL; //왼쪽 자식 NULL로 표현
  nilNode -> right = NULL; //오른쪽 자식 NULL로 표현

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t * temp_node = (node_t *)calloc(1, sizeof(node_t));
  node_t * cur; //BST 비교용 ptr

  temp_node -> color = RBTREE_RED; //삽입 노드의 색깔 설정 : 삽입 노드의 색깔은 무조건 RED
  temp_node -> key = key; //삽입 노드의 데이터 값 설정

  //현재 트리에 어떠한 노드도 없다면
  if (t-> root -> left == NULL && t-> root -> right == NULL)
  {
    t -> root = temp_node; // 루트 노드를 삽입하는 노드로 설정
    temp_node -> left = t -> nil; //삽입하는 노드의 자식들을
    temp_node -> right = t -> nil; //nil 노드로 설정
    //t -> nil -> parent = temp_node; nil 노드의 부모 노드를 현재 삽입한 노드로 설정할 필요가 없음. 어차피 노드가 하나 들어올 때마다 계속 반복하게 되니 의미 없음. 오류만 유발함.
    temp_node -> color = RBTREE_BLACK; //색깔을 black으로 변경
    temp_node -> parent = t -> nil; //삽입한 노드의 부모를 nil으로 설정(삽입 적합성을 판정할 때 필요)
  }
  //우선 BST 규칙에 맞추어 삽입한 후, RB Tree 규칙 위반 여부를 파악한다
  //이 경우 nil 노드의 부모 노드는 설정할 필요가 없을까?(설정 하기도 애매함 어차피 덮어씌어짐)
  else //그 외(하나라도 있다면)
  {
    cur = t -> root;
    while (temp_node -> left != t -> nil) //삽입하는 노드가 nil 노드를 만날때까지
    {
      if (temp_node -> key > cur -> key) //삽입하는 노드의 값이 현재 노드의 값보다 크면
      {
        if(cur -> right == t -> nil) //cur 포인터의 오른쪽이 nil 노드라면(==말단 노드라면)
        {
          cur -> right = temp_node; //cur 포인터의 오른쪽을 temp_node로 설정
          temp_node -> left = t -> nil; //삽입한 노드의 왼쪽 오른쪽을 nil로 설정
          temp_node -> right = t -> nil;
          temp_node -> parent = cur; //삽입한 노드의 부모 노드를 cur 포인터로 설정
        }
        else
          cur = cur -> right; //cur 포인터를 오른쪽으로 이동
      }
      else //삽입하는 노드의 값이 현재 노드의 값보다 작으면(같은 경우는 고려하지 않음)
      {
        if(cur -> left == t -> nil) //cur 포인터의 왼쪽이 nil 노드라면(== 말단 노드라면)
        {
          cur -> left = temp_node; //cur 포인터의 왼쪽을 temp_node로 설정
          temp_node -> left = t -> nil; //삽입한 노드의 왼쪽 오른쪽을 nil로 설정
          temp_node -> right = t -> nil;
          temp_node -> parent = cur; //삽입한 노드의 부모 노드를 cur 포인터로 설정
        }
        else
          cur = cur -> left; //cur 포인터를 왼쪽으로 이동
      }
    }
    //RB Tree 조건 체크
    rbtree_check(t, temp_node);
  }

  return temp_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t * temp = t -> root;

  while (temp != t -> nil)
  {
    //만약 검색하는 값이 temp에 저장된 값보다 크다면
    if (temp -> key < key)
    {
        temp = temp -> right;
    }
    //검색하는 값이 temp에 저장된 값보다 작다면
    else if (temp -> key > key)
    {
        temp = temp -> left;
    }
    //같다면
    else
    {
        //반복문 중지
        return temp;
    }
  }


  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}

void rbtree_check(rbtree *t, node_t *n)
{
  //삽입한 노드의 부모가 검정이 될 때까지(삽입을 함으로서 위반이 되는 경우는 루트 노드가 검정이 아닌 경우, 빨강이 연속으로 나오는 경우밖에 없음)
  //꼭대기까지 올라갈 경우 : 루트 노드의 부모는 nil 노드로 처리되어 있기에 (nil노드의 색깔은 검정) 알아서 종료됨
  while (n -> parent -> color == RBTREE_RED)
  {
      //삽입한 노드의 부모가 왼쪽 자식이라면
      if (n -> parent == n -> parent -> parent -> left)
      {
          //삼촌 노드 포인터 할당
          node_t * uncle = n -> parent -> parent -> right;
          //삼촌 노드도 빨강이면 : 부모, 삼촌 색깔 바꾸고 조부모 색깔 바꾸기
          if (uncle -> color == RBTREE_RED)
          {
              n -> parent -> color = RBTREE_BLACK;
              uncle -> color = RBTREE_BLACK;
              n -> parent -> parent -> color = RBTREE_RED;
              //검사 대상을 조부모로 옮겨서 다시 문제 체크 하기 => 조부모의 부모가 빨강색일수도 있으니까
              n = n -> parent -> parent;
          }
          //삼촌 노드는 검정이면
          else
          {
              //삽입한 노드가 오른쪽 자식이라면
              if (n == n -> parent -> right)
              {
                  //검사 대상을 부모로 옮기고
                  n = n -> parent;
                  //왼쪽 회전 실시
                  rotate_left(t, n);
              }
              //부모의 색깔을 검정으로 바꾸고
              n -> parent -> color = RBTREE_BLACK;
              //조부모의 색깔은 빨강으로 바꾼다
              n -> parent -> parent -> color = RBTREE_RED;
              //우측 회전 실시
              rotate_right(t, n);
          }
      }
      //삽입한 노드의 부모가 오른쪽 자식이라면
      if (n -> parent == n -> parent -> parent -> right)
      {
          //삼촌 노드 포인터 할당
          node_t * uncle = n -> parent -> parent -> left;
          //삼촌 노드도 빨강이면 : 부모, 삼촌 색깔 바꾸고 조부모 색깔 바꾸기
          if (uncle -> color == RBTREE_RED)
          {
              n -> parent -> color = RBTREE_BLACK;
              uncle -> color = RBTREE_BLACK;
              n -> parent -> parent -> color = RBTREE_RED;
              //검사 대상을 조부모로 옮겨서 다시 문제 체크 하기 => 조부모의 부모가 빨강색일수도 있으니까
              n = n -> parent -> parent;
          }
          //삼촌 노드는 검정이면
          else
          {
              //삽입한 노드가 오른쪽 자식이라면
              if (n == n -> parent -> left)
              {
                  //검사 대상을 부모로 옮기고
                  n = n -> parent;
                  //오른쪽 회전 실시
                  rotate_right(t, n);
              }
              //부모의 색깔을 검정으로 바꾸고
              n -> parent -> color = RBTREE_BLACK;
              //조부모의 색깔은 빨강으로 바꾼다
              n -> parent -> parent -> color = RBTREE_RED;
              //좌측 회전 실시
              rotate_left(t, n);
          }
      }
  }
  //2번 룰을 위반하지 않게끔 루트 노드 색깔을 검정색으로 처리
  t -> root -> color = RBTREE_BLACK;

}

node_t * rbtree_travel(rbtree *t, node_t *n) //문제가 발생한 노드의 포인터를 반환
{
  if (n == t -> nil) //닐 노드까지 도달했다면
  {
    return n; //반복 종료. 닐 노드 포인터 반환
  }
  //만약 탐색 중인 노드의 색깔이 빨강인데 부모 색깔도 빨강이면
  if (n -> color == RBTREE_RED && n -> parent -> color == RBTREE_RED)
  {
    return n; //해당 노드 포인터 반환
  }
}

//왼쪽 회전 구현 : 색깔 변경은 나중에 하기
//회전하는 주체 : 조부모 노드
void rotate_left(rbtree *t, node_t *n)
{
    //조부모의 오른쪽 자식
    node_t * child = n -> right;
    //오른쪽 자식의 왼쪽 자식 == 손자
    node_t * grandchild = child -> left;

    //손자 기준 조부모가 부모의 왼쪽 자식이 되고
    //기존 부모의 왼쪽 자식은 조부모의 오른쪽 자식이 된다

    //조부모의 오른쪽 자식을 부모의 왼쪽 자식으로 바꾼다
    n -> right = child -> left;

    if (child -> left != t -> nil) //부모의 왼쪽 자식이 nil이 아니라면
    {
        child -> left -> parent = n; // 부모의 왼쪽 서브트리의 부모를 조부모 노드(현재 노드)로 한다
    }

    //RB Tree의 노드에는 부모에 대한 정보도 있기 때문에
    //부모에 대한 정보도 모두 조정을 해줘야 한다

    //부모의 부모를 조부모의 부모로 설정한다
    child -> parent = n -> parent;

    //조부모가 루트였던 경우
    if (n -> parent == NULL)
    {
        //부모를 루트로 설정한다
        t -> root = child;
    }
    //조부모가 왼쪽 자식이었던 경우
    else if (n -> parent -> left == n)
    {
        //이제 child가 왼쪽 자식이 된다
        n -> parent -> left = child;
    }
    //조부모가 오른쪽 자식이었던 경우
    else
    {
        //이제 child가 오른쪽 자식이 된다
        n -> parent -> right = child;
    }

    //조부모가 이제 부모의 왼쪽 자식이 된다
    child -> left = n;
    //조부모의 부모 설정도 해준다
    n -> parent = child;
}

//오른쪽 회전 구현 : 색깔 변경은 나중에 하기
//회전하는 주체 : 조부모 노드
void rotate_right(rbtree *t, node_t *n)
{
    //조부모의 왼쪽 자식
    node_t * child = n -> left;
    //왼쪽 자식의 오른쪽 자식 == 손자
    node_t * grandchild = child -> right;

    //손자 기준 조부모가 부모의 오른쪽 자식이 되고
    //기존 부모의 오른쪽 자식은 조부모의 왼쪽 자식이 된다

    //조부모의 왼쪽 자식을 부모의 오른쪽 자식으로 바꾼다
    n -> left = child -> right;

    if (child -> right != t -> nil) //부모의 오른쪽 자식이 nil이 아니라면
    {
        child -> right -> parent = n; // 부모의 오른쪽 서브트리의 부모를 조부모 노드(현재 노드)로 한다
    }

    //RB Tree의 노드에는 부모에 대한 정보도 있기 때문에
    //부모에 대한 정보도 모두 조정을 해줘야 한다

    //부모의 부모를 조부모의 부모로 설정한다
    child -> parent = n -> parent;

    //조부모가 루트였던 경우
    if (n -> parent == NULL)
    {
        //부모를 루트로 설정한다
        t -> root = child;
    }
    //조부모가 왼쪽 자식이었던 경우
    else if (n -> parent -> left == n)
    {
        //이제 child가 왼쪽 자식이 된다
        n -> parent -> left = child;
    }
    //조부모가 오른쪽 자식이었던 경우
    else
    {
        //이제 child가 오른쪽 자식이 된다
        n -> parent -> right = child;
    }

    //조부모가 이제 부모의 오른쪽 자식이 된다
    child -> right = n;
    //조부모의 부모 설정도 해준다
    n -> parent = child;
}
