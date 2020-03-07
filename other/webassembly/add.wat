(module
  (type (;0;) (func (param i32 i32) (result i32)))
  (func $add (type 0) (param i32 i32) (result i32)
    get_local 1
    get_local 0
    i32.add)
  (memory 1)
  (export "memory" (memory 0))
  (export "add" (func $add))
)
