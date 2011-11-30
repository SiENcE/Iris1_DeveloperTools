unit Unit_Settings;
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

uses
  Graphics, Registry, Windows, SysUtils, Forms;

var
   Setting_BackgroundColor: TColor;
   Setting_TextureDir: String;

   procedure SaveSettings;
   procedure LoadSettings;

implementation

procedure SaveSettings;
var
  Reg: TRegistry;
begin
   Reg := TRegistry.Create;
   try
      Reg.RootKey := HKEY_CURRENT_USER;
      Reg.OpenKey ('Software\the Gremium\Ultima Iris\Particle Editor\', True);
      Reg.WriteInteger ('BackgroundColor', Setting_BackgroundColor);
      Reg.WriteString ('TextureDir', Setting_TextureDir);
   finally
      Reg.Free;
   end;
end;

procedure LoadSettings;
var
  Reg: TRegistry;
begin
   Reg := TRegistry.Create;
   try
      Reg.RootKey := HKEY_CURRENT_USER;
      if Reg.OpenKey ('Software\the Gremium\Ultima Iris\Particle Editor\', False) then begin
         if Reg.ValueExists ('BackgroundColor') then
            Setting_BackgroundColor := Reg.ReadInteger ('BackgroundColor');
         if Reg.ValueExists ('TextureDir') then
            Setting_TextureDir := IncludeTrailingBackslash(Reg.ReadString ('TextureDir'));
      end;
   finally
      Reg.Free;
   end;
end;

initialization
  Setting_BackgroundColor := clBlack;
  Setting_TextureDir := '';

finalization


end.
