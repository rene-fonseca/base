Distribute evenly vertically
Distribute fixed vertically (specify distance)
Distribute fixed horizontally (specify distance)
Distribute evenly horizontally

Dynamic/fixed dimension
Dynamic/fixed position (function of alignment and position)

Position getPosition() const throw(); ????
Dimension getDimension() const throw();

enum HorizontalAlignment {
  LEFT,
  CENTER,
  RIGHT
};

enum VerticalAlignment {
  TOP,
  MIDDLE,
  BOTTOM
};

get/set minimum dimension
get/set maximum dimension

// disable/enable to prevent multiple unnecessary calcs.
void disable();
void enable();

void add(Object object) throw(?);
void remove(Object object) throw(?);

void onResize(const Dimension& dimension) throw();
// calc new positions and dimensions and call update?

// concept of binding point (inherited from placement operator and set explicitly when added)

HorizontalAlignment getHorizontalAlignment() const throw();
VerticalAlignment getVerticalAlignment() const throw();

// [0; 9]?
int getAlignment() const throw();

class _DK_SDU_MIP__BASE__API PlacementOperator : public UIObject {
public:

  void disable() throw();
  void enable() throw();
  bool isEnabled() const throw();
};

class _DK_SDU_MIP__BASE__API VerticalPlacement : public Object {
public:

  VerticalPlacement() throw();
  VerticalPlacement(unsigned int distance) throw();

  unsigned int getDistance();
  void setDistance();
  
  void add(UIObject object, const Position& position) throw();
};

class _DK_SDU_MIP__BASE__API HorizontalPlacement : public Object {
public:
  
};

class _DK_SDU_MIP__BASE__API CheckboardPlacement : public Object {
public:
  
  CheckboardPlacement() throw();
  CheckboardPlacement(const Dimension& dimension) throw();
  Dimension getRowsAndColumns() const throw();
  
  // specify the (row, column) with add?
  bool hasObject(UIObject object) const throw();
  bool hasObject(const Position& position) const throw();
  void add(UIObject object, const Position& position) throw();
  void remove(const Position& postion) throw();
  void remove(UIObject object) throw();
};

// Tabulator - automatic starting corner UPPER_LEFT or LOWER_RIGHT (vertical or horizontal priority)
