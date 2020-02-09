; ModuleID = 'My module'
source_filename = "My module"

@0 = private unnamed_addr constant [6 x i8] c"\22A: \22\00", align 1
@1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@4 = private unnamed_addr constant [10 x i8] c"\22RESULT:\22\00", align 1
@5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @D(i32 %b, i32 %a, i32 %c) {
entry:
  %c3 = alloca i32
  %a2 = alloca i32
  %b1 = alloca i32
  store i32 %b, i32* %b1
  store i32 %a, i32* %a2
  store i32 %c, i32* %c3
  %res = alloca i32
  %b4 = load i32, i32* %b1
  %b5 = load i32, i32* %b1
  %multmp = mul i32 %b4, %b5
  %a6 = load i32, i32* %a2
  %multmp7 = mul i32 4, %a6
  %c8 = load i32, i32* %c3
  %multmp9 = mul i32 %multmp7, %c8
  %subtmp = sub i32 %multmp, %multmp9
  store i32 %subtmp, i32* %res
  %res10 = load i32, i32* %res
  ret i32 %res10
}

define i32 @main() {
entry:
  %a = alloca i32
  store i32 5, i32* %a
  %flag = alloca i32
  store i32 1, i32* %flag
  %println = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0))
  br label %loop

loop:                                             ; preds = %ifcont, %entry
  %a1 = load i32, i32* %a
  %lesstmp = icmp slt i32 %a1, 11
  br i1 %lesstmp, label %loop1, label %afterloop

loop1:                                            ; preds = %loop
  %flag2 = load i32, i32* %flag
  %eqtmp = icmp eq i32 %flag2, 1
  br i1 %eqtmp, label %iftrue, label %iffalse

afterloop:                                        ; preds = %loop
  br label %cond

iftrue:                                           ; preds = %loop1
  %a3 = load i32, i32* %a
  %println4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @2, i32 0, i32 0), i32 %a3)
  br label %ifcont

iffalse:                                          ; preds = %loop1
  %a5 = load i32, i32* %a
  %addtmp = add i32 %a5, 2
  store i32 %addtmp, i32* %a
  br label %ifcont

ifcont:                                           ; preds = %iffalse, %iftrue
  %a6 = load i32, i32* %a
  %addtmp7 = add i32 %a6, 1
  store i32 %addtmp7, i32* %a
  br label %loop

cond:                                             ; preds = %loop9, %afterloop
  %a8 = load i32, i32* %a
  %geetmp = icmp sge i32 %a8, 0
  br i1 %geetmp, label %loop9, label %afterloop14

loop9:                                            ; preds = %cond
  %a10 = load i32, i32* %a
  %println11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @3, i32 0, i32 0), i32 %a10)
  %a12 = load i32, i32* %a
  %addtmp13 = add i32 %a12, 1
  store i32 %addtmp13, i32* %a
  br label %cond

afterloop14:                                      ; preds = %cond
  %println15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @5, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @4, i32 0, i32 0))
  %a16 = load i32, i32* %a
  %calltmp = call i32 @D(i32 10, i32 %a16, i32 1)
  %println17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @6, i32 0, i32 0), i32 %calltmp)
  ret i32 0
}
