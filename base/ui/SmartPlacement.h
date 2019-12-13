Distribute evenly vertically
Distribute fixed vertically (specify distance)
Distribute fixed horizontally (specify distance)
Distribute evenly horizontally

Dynamic/fixed dimension
Dynamic/fixed position (function of alignment and position)

Position getPosition() const noexcept; ????
Dimension getDimension() const noexcept;

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

void onResize(const Dimension& dimension) noexcept;
// calc new positions and dimensions and call update?

// concept of binding point (inherited from placement operator and set explicitly when added)

HorizontalAlignment getHorizontalAlignment() const noexcept;
VerticalAlignment getVerticalAlignment() const noexcept;

// [0; 9]?
int getAlignment() const noexcept;

class _COM_AZURE_DEV__BASE__API PlacementOperator : public UIObject {
public:

  void disable() noexcept;
  void enable() noexcept;
  bool isEnabled() const noexcept;
};

class _COM_AZURE_DEV__BASE__API VerticalPlacement : public Object {
public:

  VerticalPlacement() noexcept;
  VerticalPlacement(unsigned int distance) noexcept;

  unsigned int getDistance();
  void setDistance();
  
  void add(UIObject object, const Position& position) noexcept;
};

class _COM_AZURE_DEV__BASE__API HorizontalPlacement : public Object {
public:
  
};

class _COM_AZURE_DEV__BASE__API CheckboardPlacement : public Object {
public:
  
  CheckboardPlacement() noexcept;
  CheckboardPlacement(const Dimension& dimension) noexcept;
  Dimension getRowsAndColumns() const noexcept;
  
  // specify the (row, column) with add?
  bool hasObject(UIObject object) const noexcept;
  bool hasObject(const Position& position) const noexcept;
  void add(UIObject object, const Position& position) noexcept;
  void remove(const Position& postion) noexcept;
  void remove(UIObject object) noexcept;
};

// Tabulator - automatic starting corner UPPER_LEFT or LOWER_RIGHT (vertical or horizontal priority)
