(module
  (import "math" "add" (func $add (param i32 i32) (result i32)))
  (type (;0;) (func (param i32 i32) (result i32)))
  (memory 1)
  (export "memory" (memory 0))
  (func $main (export "_start")
    (call $add
      (i32.const 19281)
      (i32.const 12342)
    )
    drop
  )
  (func $_add (param $lhs i32) (param $rhs i32) (result i32)
    local.get $lhs
    local.get $rhs
    call $add
  )
  (export "_add" (func $_add))
)
