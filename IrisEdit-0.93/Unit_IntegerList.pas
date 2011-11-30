unit Unit_IntegerList;
//
// Created by: Alexander Oster - tensor@ultima-iris.de
//
(*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****)

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
