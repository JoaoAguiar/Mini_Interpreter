typedef enum {ATRIB, ADD, SUB, MUL, DIV, IF_GOTO, PRINT, READ, GOTO, LABEL, QUIT, NOTHING} OpKind;

typedef enum {EMPTY, INT_CONST, STRING} ElemKind;

typedef struct {
  ElemKind kind;

  union{
    int value;
    char* name;
  } content;
} Elem;

typedef struct {
  OpKind operation;
  Elem firstElement, secondElement, thirdElement;
} Instr;

typedef struct list {
  Instr instruction;
  struct list* nextInstruction;
}* List;

Elem mkVar(char* s);
Elem mkInt(int n);
Instr mkInstr(OpKind oper, Elem x, Elem y, Elem z);
Elem empty();
List mkList(Instr head, List tail);
List jumpLine(List aux, char* name);
void runData(List copy, List list);
List readData(FILE* file);
List addLast(Instr val, List l);
List tail(List l);
int getValue(Elem element);
