{==============================================================================

  Iris Model Editor - Copyright by Alexander Oster, tensor@ultima-iris.de

 The contents of this file are used with permission, subject to
 the Mozilla Public License Version 1.1 (the "License"); you may
 not use this file except in compliance with the License. You may
 obtain a copy of the License at
 http://www.mozilla.org/MPL/MPL-1.1.html

 Software distributed under the License is distributed on an
 "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 implied. See the License for the specific language governing
 rights and limitations under the License.

==============================================================================}

unit Unit_IntegerList;

interface

uses Windows, SysUtils, Classes, Graphics, extctrls;

type
  TIntegerList = class (TList)
    private
      function GetItem(Index: Integer): Integer;
      procedure SetItem(Index, Value: Integer);
    public
      property Items[Index: Integer]: Integer read GetItem write SetItem; default;
      procedure Add (Value: Integer);
  end;

implementation

function TIntegerList.GetItem(Index: Integer): Integer;
begin
     result := Integer(inherited Items[Index]);
end;

procedure TIntegerList.SetItem(Index, Value: Integer);
begin
     inherited Items[Index] := Pointer(Value);
end;

procedure TIntegerList.Add (Value: Integer);
begin
     inherited Add(Pointer(Value));
end;


end.
