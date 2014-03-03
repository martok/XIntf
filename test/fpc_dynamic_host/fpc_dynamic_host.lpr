program fpc_dynamic_host;

uses
  SysUtils, windows;

type
  IAdder = interface
    ['{39000AE7-F922-4041-AB6C-AE20FB802C3B}']

    function Add(A, B: DWORD): DWORD; stdcall;
  end;

  IMultiplier = interface
    ['{39000AE7-F922-4041-AB6C-AE20FB802C3C}']

    function Times(A, B: DWORD): DWORD; stdcall;
  end;


function GetInstance(out inst: IAdder): LongInt; stdcall; external 'cpp_dynamic_lib.dll';


procedure Test;
var
  adder: IAdder;
  multiplier: IMultiplier;
begin
  if GetInstance(adder) = 0 then begin
    WriteLn('Got Adder!');
    WriteLn('42 + 23 = ', adder.Add(42,23));

    if Supports(adder, IMultiplier, multiplier) then begin
      WriteLn('Is also a Multiplier!');
      WriteLn('42 * 23 = ', multiplier.Times(42,23));
    end;

  end;
end;

begin
  Test;
  Write('Press any key...');
  ReadLn;
end.

