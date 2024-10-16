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

void delete_rbtree_whole(rbtree *t, node_t * cur)
{
    if (cur == t -> nil)
      return;
    delete_rbtree_whole(t, cur -> left);
    delete_rbtree_whole(t, cur -> right);
    free(cur);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  delete_rbtree_whole(t, t -> root);
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
  node_t * temp = t -> root;
  while (temp -> left != t -> nil)
  {
      temp = temp -> left;
  }

  return temp;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t * temp = t -> root;
  while (temp -> right != t -> nil)
  {
      temp = temp -> right;
  }

  return temp;
}

//서브트리의 min 값을 찾아주는 함수
node_t *subtree_min(rbtree *t, node_t *n) {
  // TODO: implement find
  node_t * temp = n;
  while (temp -> left != t -> nil)
  {
      temp = temp -> left;
  }

  return temp;
}

//왼쪽 회전 구현 : 색깔 변경은 나중에 하기
//회전하는 주체 : 조부모 노드
void rotate_left(rbtree *t, node_t *n)
{
    //조부모의 오른쪽 자식
    node_t * child = n -> right;

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
    if (n -> parent == t->nil)
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
    if (n -> parent == t -> nil)
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

//삭제 후 해당 트리가 RB Tree의 규칙을 지키고 있는지를 체크하는 함수
void rbtree_erase_check (rbtree *t, node_t *x)
{
    
    node_t * w;
    //체크하는 노드가 루트가 아니고, 색깔이 검정일 때까지
    //체크하는 노드가 루트에 다다르거나, 색깔이 빨강이 되는 순간 break
    //x는 무조건 색깔이 검정인 상태로 들어옴. 이유: 이 함수 호출 전 조건이 그렇기 때문
    while (x != t -> root && x -> color == RBTREE_BLACK)
    {
        //현재 체크하는 노드가 왼쪽 자식이면
        if (x == x -> parent -> left)
        {
            //오른쪽 자식(형제)를 가져오기
            w = x -> parent -> right;
            //만약 형제가 빨강이라면 : 검정 높이가 달라질 우려가 있음
            if (w -> color == RBTREE_RED)
            {
                w -> color = RBTREE_BLACK; //형제 색깔도 검사 노드와 같게 검정으로 바꾸기
                x -> parent -> color = RBTREE_RED; //부모 색깔은 빨강으로 바꾸기
                rotate_left(t, x -> parent); //부모를 축으로 좌회전 => 결과 : 기존 w가 조부모(서브 트리의 루트)가 됨
                w = x -> parent -> right; //w가 추적하는 대상을 부모의 오른쪽 자식으로 변경 -> 여전히 x의 형제를 추적중
            }
        
            //x의 형제의 왼쪽 자식 이 검정이고 오른쪽 자식도 검정이면
            //5번 룰 위반. 형제 쪽으로 타고 갈 경우가 검정 높이가 1만큼 더 높다
            if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK)
            { 
                //형제의 색깔을 빨강으로 변경
                w -> color = RBTREE_RED;
                //추적 대상을 부모로 변경
                x = x -> parent;
            }
            //x의 형제의 자식들 중 하나가 빨강이면
            else
            {   
                //형제의 오른쪽 자식이 검정이면 (=왼쪽 자식이 빨강이면)
                if (w -> right -> color == RBTREE_BLACK)
                {   
                    //형제 왼쪽 자식도 검정으로 바꾸기
                    w -> left -> color = RBTREE_BLACK;
                    //형제의 색깔은 빨강으로 변경
                    w -> color = RBTREE_RED;
                    //형제를 축으로 오른쪽 회전
                    rotate_right(t, w);
                    //w 포인터의 추적 대상을 현재 x의 형제(=원래 w의 왼쪽 자식)로 변경
                    w = x -> parent -> right;
                }
                //형제의 색깔을 부모의 색깔로 변경
                w -> color = x -> parent -> color;
                //부모의 색깔은 겸정으로 변경
                x -> parent -> color = RBTREE_BLACK;
                w -> right -> color = RBTREE_BLACK;
                rotate_left(t, x -> parent);
                x = t -> root;
            }
        }
        //현재 체크하는 노드가 오른쪽 자식이면
        else
        {
            //왼쪽 자식(형제)를 가져오기
            w = x -> parent -> left;
            //만약 형제가 빨강이라면 : 검정 높이가 달라질 우려가 있음
            if (w -> color == RBTREE_RED)
            {
                w -> color = RBTREE_BLACK; //형제 색깔도 검사 노드와 같게 검정으로 바꾸기
                x -> parent -> color = RBTREE_RED; //부모 색깔은 빨강으로 바꾸기
                rotate_right(t, x -> parent); //부모를 축으로 우회전 => 결과 : 기존 w가 조부모(서브 트리의 루트)가 됨
                w = x -> parent -> left; //w가 추적하는 대상을 부모의 왼쪽 자식으로 변경 -> 여전히 x의 형제를 추적중
            }
        
            //x의 형제의 오른쪽 자식 이 검정이고 왼쪽 자식도 검정이면
            //5번 룰 위반. 형제 쪽으로 타고 갈 경우가 검정 높이가 1만큼 더 높다
            if (w -> left -> color == RBTREE_BLACK && w -> right -> color == RBTREE_BLACK)
            { 
                //형제의 색깔을 빨강으로 변경
                w -> color = RBTREE_RED;
                //추적 대상을 부모로 변경
                x = x -> parent;
            }
            //x의 형제의 자식들 중 하나가 빨강이면
            else
            {   
                //형제의 왼쪽 자식이 검정이면 (=오른쪽 자식이 빨강이면)
                if (w -> left -> color == RBTREE_BLACK)
                {   
                    //형제 오른쪽 자식도 검정으로 바꾸기
                    w -> right -> color = RBTREE_BLACK;
                    //형제의 색깔은 빨강으로 변경
                    w -> color = RBTREE_RED;
                    //형제를 축으로 오른쪽 회전
                    rotate_left(t, w);
                    //w 포인터의 추적 대상을 현재 x의 형제(=원래 w의 왼쪽 자식)로 변경
                    w = x -> parent -> left;
                }
                //형제의 색깔을 부모의 색깔로 변경
                w -> color = x -> parent -> color;
                //부모의 색깔은 겸정으로 변경
                x -> parent -> color = RBTREE_BLACK;
                w -> left -> color = RBTREE_BLACK;
                rotate_right(t, x -> parent);
                x = t -> root;
            }
        }
    }
    //루트 노드를 검정으로 설정
    x -> color = RBTREE_BLACK;
}


//삭제한 노드 자리에 삭제한 노드의 자식을 넣는 작업
//매개변수 순서 : 트리, 삭제할 노드, 이제 서브 트리의 루트가 될 노드(삭제할 노드의 자식 노드)
void rbtree_transplant(rbtree *t, node_t *delete_node, node_t * will_be_root)
{
    //삭제한 노드가 루트 노드였다면
    if(delete_node -> parent == t -> nil)
    {
        //트리의 루트를 삭제할 노드의 자식 노드로 설정
        t -> root = will_be_root;
    }
    //삭제할 노드가 왼쪽 자식이라면
    else if (delete_node == delete_node -> parent -> left)
    {   
        //삭제할 노드의 자식 노드를 왼쪽 자식으로 설정
        delete_node -> parent -> left = will_be_root;
    }
    //오른쪽 자식이라면
    else
    {   
        //삭제할 노드의 자식 노드를 오른쪽 자식으로 설정
        delete_node -> parent -> right = will_be_root;
    }
    //삭제할 노드의 부모 노드를 삭제할 노드의 부모로 설정
    will_be_root -> parent = delete_node -> parent;
}


//rbtree의 노드를 삭제하는 작업
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t * temp = p;
  node_t * child;
  //삭제하는 노드의 색깔을 저장하는 변수 선언
  color_t original_color = temp -> color;

  //삭제하는 노드의 좌측 자식이 nil 노드라면
  if (p -> left == t -> nil)
  {
      //삭제하는 노드의 우측 자식을 받아오기 (미리 받아오는 이유 : 삭제한 이후 rbtree 규칙을 체크할 때 필요함)
      child = p -> right;
      rbtree_transplant(t, p, p -> right);
  }
  //삭제하는 노드의 우측 자식이 nil 노드라면
  else if (p -> right == t -> nil)
  {
      //삭제하는 노드의 좌측 자식을 받아오기 (미리 받아오는 이유 : 삭제한 이후 rbtree 규칙을 체크할 때 필요함)
      child = p -> left;
      rbtree_transplant(t, p, p -> left);
  }
  //삭제하려는 노드의 양 쪽에 모두 자식이 있다면
  else
  {
      //삭제하려는 노드의 오른쪽 자식이 루트인 서브트리에서, 삭제하려는 노드보다 값이 큰 노드들 중 가장 작은 노드를 저장
      node_t * next_minimal = subtree_min(t, p -> right);
      //original color는 실제로 이동하는 노드의 색깔을 저장
      //삭제된 노드의 자리로 이동한 후 자리를 채우는 노드는 색깔을 계승받지 않기 때문에
      //기존 노드 색깔을 토대로 판단해야 함
      original_color = next_minimal -> color;
      child = next_minimal -> right;
      //next_minimal이 삭제하려는 노드의 오른쪽 (직계)자식이 아니라면
      if (next_minimal != p -> right)
      {   
          //next_minimal을 삭제하고 그 자리에 next_minimal의 오른쪽 자식(=next_minimal 다음으로 가장 작은 노드) 삽입
          rbtree_transplant(t, next_minimal, next_minimal -> right);
          //next_minimal의 오른쪽 자식을 삭제하려는 노드의 오른쪽 자식으로 교체
          next_minimal -> right = p -> right;
          //삭제하려는 노드의 오른쪽 자식의 부모를 next_minimal로 교체
          next_minimal -> right -> parent = next_minimal;
      }
      //next_minimal이 삭제하려는 노드의 오른쪽 직계 자식이라면
      else
      {
          //next_minimal의 successor의 부모를 next_minimal로 설정
          child -> parent = next_minimal;
      }
      //next_minimal을 삭제할 노드 자리에 이식
      rbtree_transplant(t, p, next_minimal);
      //next_minimal이 삭제한 노드의 왼쪽 자식들을 입양받음
      next_minimal -> left = p -> left;
      //삭제한 노드의 왼쪽 자식들의 부모를 next_minimal로 설정
      next_minimal -> left -> parent = next_minimal;
      //색깔 계승받기
      next_minimal -> color = p -> color;
  }

  //삭제한 노드 메모리 동적 해제
  free(p);

  //origin color가 black이었을 경우를 조사하는 이유
  //원래 빨강이라면 RB Tree의 특성이 유지될 수밖에 없음
  //검정 높이는 모두 그대로 유지되고(높이를 셀 때 빨강이 사라졌다고 해서 높이가 달라지진 않으니)
  //빨강과 빨강이 연속될 일도 없음 (애초에 정상적인 RB Tree였다면 빨강의 자식 노드가 빨강일 리가 없음)
  //삭제 이전 RB Tree가 규칙을 모두 유지했다 했을 때, 루트가 빨강이 아니었을 것임
  if (original_color == RBTREE_BLACK)
      rbtree_erase_check(t, child);

  return 0;
}

int tree_to_array(const rbtree *t, key_t *arr, const size_t n, node_t * cur, int * count)
{
    //현재 도달한 노드가 nil 노드라면
    if (cur == t -> nil)
      //함수 종료
      return 0;
        
    tree_to_array(t, arr, n, cur -> left, count);
    //배열에 현재 노드의 key값 삽입
    arr[(*count)] = cur -> key;
    //count값 추가
    *count += 1;
    tree_to_array(t, arr, n, cur -> right, count);
    return 0;

}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t -> root == t -> nil)
    return 0;
  
  int count = 0;

  tree_to_array(t, arr, n, t -> root, &count);

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
              rotate_right(t, n -> parent -> parent);
          }
      }
      //삽입한 노드의 부모가 오른쪽 자식이라면
      else
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
              rotate_left(t, n -> parent -> parent);
          }
      }
  }
  //2번 룰을 위반하지 않게끔 루트 노드 색깔을 검정색으로 처리
  t -> root -> color = RBTREE_BLACK;

}