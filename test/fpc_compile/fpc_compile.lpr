program fpc_compile;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Classes,SysUtils;

type
  (* Interface declaration *)
  ITestInterface = interface
    ['{39000AE7-F922-4041-AB6C-AE20FB802C3A}']
    function Add(const A, B: DWORD): DWORD; stdcall;
  end;

  (* Implement interface and IUnknown via TInterfacedObject *)
  TTestClass = class(TInterfacedObject, ITestInterface)
    function Add(const A, B: DWORD): DWORD; stdcall;
  end;

function TTestClass.Add(const A, B: DWORD): DWORD; stdcall;
begin
  Result:= A + B;
end;

(* Pass in as IUnknown and try getting specific one just to test if that works *)
procedure TestAdder(const intf: IUnknown);
var
  iTest: ITestInterface;
  s: DWORD;
begin
  if intf.QueryInterface(ITestInterface, iTest) = S_OK then begin
    s:= iTest.Add(42, 23);
    WriteLn('Sum of 42 + 23 = ', s);
  end;
end;

(* Construct an object, store refcounted interface pointer
   Automatically frees adder at function exit (scope end)
*)
procedure RunTest;
var
  adder: ITestInterface;
begin
  adder:= TTestClass.Create;

  (* implicit cast of specific interface to IUnknown *)
  TestAdder(adder);
end;

begin
  RunTest;
end.

