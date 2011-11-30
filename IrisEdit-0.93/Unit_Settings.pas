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
   BackgroundColor: TColor;
   LineColor: TColor;
   PointColor: TColor;
   SelectedPointColor: TColor;
   GroundColor: TColor;
   FaceColor: TColor;
   VisibleBackFaceColor: TColor;
   InvisibleBackFaceColor: TColor;

   Render_Points: Boolean;
   Render_WireFrame: Boolean;
   Render_Faces: Boolean;
   Render_BackFaces: Boolean;
   Render_Outbox: Boolean;
   Render_Ground: Boolean;
   Render_Textures: Boolean;

   TileDataMul: String;
   ArtMul: String;
   ArtIdxMul: String;

   ColorDepth: Integer = 16;
   ZBufferDepth: Integer = 16;

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
      Reg.OpenKey ('Software\the Gremium\Ultima Iris\Model Editor\', True);
      Reg.WriteInteger ('BackgroundColor', BackgroundColor);
      Reg.WriteInteger ('VisibleBackfaceColor', VisibleBackfaceColor);
      Reg.WriteInteger ('InvisibleBackfaceColor', InvisibleBackfaceColor);
      Reg.WriteInteger ('GroundColor', GroundColor);
      Reg.WriteInteger ('PointColor', PointColor);
      Reg.WriteInteger ('SelectedPointColor', SelectedPointColor);
      Reg.WriteInteger ('LineColor', LineColor);
      Reg.WriteInteger ('ColorDepth', ColorDepth);
      Reg.WriteInteger ('ZBufferDepth', ZBufferDepth);
      Reg.WriteBool ('RenderPoints', Render_Points);
      Reg.WriteBool ('RenderWireframe', Render_Wireframe);
      Reg.WriteBool ('RenderFaces', Render_Faces);
      Reg.WriteBool ('RenderBackfaces', Render_Backfaces);
      Reg.WriteBool ('RenderOutbox', Render_Outbox);
      Reg.WriteBool ('RenderGround', Render_Ground);
      Reg.WriteBool ('RenderTextures', Render_Textures);
      Reg.WriteString ('TileData Path', TileDataMul);
      Reg.WriteString ('Art Path', ArtMul);
      Reg.WriteString ('ArtIdx Path', ArtIdxMul);
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
      if Reg.OpenKey ('Software\the Gremium\Ultima Iris\Model Editor\', False) then begin
         if Reg.ValueExists ('BackgroundColor') then
            BackgroundColor := Reg.ReadInteger ('BackgroundColor');
         if Reg.ValueExists ('GroundColor') then
            GroundColor := Reg.ReadInteger ('GroundColor');
         if Reg.ValueExists ('PointColor') then
            PointColor := Reg.ReadInteger ('PointColor');
         if Reg.ValueExists ('ColorDepth') then
            ColorDepth := Reg.ReadInteger ('ColorDepth');
         if Reg.ValueExists ('ZBufferDepth') then
            ZBufferDepth := Reg.ReadInteger ('ZBufferDepth');
         if Reg.ValueExists ('VisibleBackfaceColor') then
            VisibleBackfaceColor := Reg.ReadInteger ('VisibleBackfaceColor');
         if Reg.ValueExists ('InvisibleBackfaceColor') then
            InvisibleBackfaceColor := Reg.ReadInteger ('InvisibleBackfaceColor');
         if Reg.ValueExists ('SelectedPointColor') then
            SelectedPointColor := Reg.ReadInteger ('SelectedPointColor');
         if Reg.ValueExists ('LineColor') then
            LineColor := Reg.ReadInteger ('LineColor');
         if Reg.ValueExists ('RenderPoints') then
            Render_Points := Reg.ReadBool ('RenderPoints');
         if Reg.ValueExists ('RenderWireframe') then
            Render_Wireframe := Reg.ReadBool ('RenderWireframe');
         if Reg.ValueExists ('RenderFaces') then
            Render_Faces := Reg.ReadBool ('RenderFaces');
         if Reg.ValueExists ('RenderBackfaces') then
            Render_Backfaces := Reg.ReadBool ('RenderBackfaces');
         if Reg.ValueExists ('RenderOutbox') then
            Render_Outbox := Reg.ReadBool ('RenderOutbox');
         if Reg.ValueExists ('RenderGround') then
            Render_Ground := Reg.ReadBool ('RenderGround');
         if Reg.ValueExists ('RenderTextures') then
            Render_Textures:= Reg.ReadBool ('RenderTextures');
         if Reg.ValueExists ('TileData Path') then
            TileDataMul := Reg.ReadString ('TileData Path');
         if Reg.ValueExists ('Art Path') then
            ArtMul := Reg.ReadString ('Art Path');
         if Reg.ValueExists ('ArtIdx Path') then
            ArtIdxMul := Reg.ReadString ('ArtIdx Path');
      end;
   finally
      Reg.Free;
   end;
end;

initialization
  BackgroundColor := $FF9494;
  GroundColor := $606060;
  PointColor := $AAAAAA;
  SelectedPointColor := $00FF00;
  LineColor := $000000;
  FaceColor := $AAAAAA;
  InvisibleBackFaceColor := $AA0000;
  VisibleBackFaceColor := $00AAAA;

  Render_Points := True;
  Render_Wireframe := True;
  Render_Faces := True;
  Render_Backfaces := True;
  Render_Outbox := True;
  Render_Ground := True;
  Render_Textures := True;

  TileDataMul := '.\TileData.mul';
  ArtMul := '.\Art.mul';
  ArtIdxMul := '.\ArtIdx.mul';


finalization


end.
