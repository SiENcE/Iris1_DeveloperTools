unit Unit_OpenGLWrapper;
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

uses Windows, Unit_OpenGL, Dialogs, Unit_OpenGLGeometry, Classes, SysUtils, Graphics;

const
  FarClipping   = -10.0;
  NearClipping  = 0.01;

type
 TGLEngine = class (TObject)
  protected
   FInitialized: Boolean;
   FHandle: HDC;
   FRC : HGLRC;
   FWidth, FHeight: Integer;
   procedure SetPixelFormat(Handle: HDC);
  public
   property Initialized: Boolean read FInitialized;
   constructor Create;
   destructor Destroy; override;
   function StartGL(Handle: HDC): Boolean;
   procedure SetupGL;
   procedure DeInit;
   procedure SetViewPort(X, Y, Width, Height: Integer);
   procedure ResetMaterial;
   procedure MakeCurrent (AHandle: HDC);
   procedure SwapBuffers (AHandle: HDC);
   procedure SetClearColor (Color: TColor);
 end;

 const
    light_position : array[0..3] of TGLfloat = (-2.0, -2.0, 2.0, 0.0);
    light_ambient : array[0..3] of TGLfloat = (0.70, 0.70, 0.70, 1.0);
    mat_specular  : array[0..3] of TGLfloat = (0.2, 0.2, 0.2, 1.0);
    mat_shininess : array[0..0] of TGLfloat = (0.2);

    mat_ambient : array[0..3] of TGLfloat = (0.9, 0.9, 0.9, 1.0);
    mat_diffuse : array[0..3] of TGLfloat = (0.7, 0.7,  0.7, 1.0);
    mat_green : array[0..3] of TGLfloat = (0.0, 0.8, 0.0, 1.0);
    mat_yellow : array[0..3] of TGLfloat = (0.0, 0.8, 0.8, 1.0);
    mat_red : array[0..3] of TGLfloat = (0.8, 0.0, 0.0, 1.0);

    procedure SetColor (AColor: TColor);
    procedure SetLightColor (Color: TColor);

implementation


constructor TGLEngine.Create;
begin
 FInitialized:=False;
 FRC := 0;
end;

destructor TGLEngine.Destroy;
begin
 if FInitialized then Deinit;
end;

function TGLEngine.StartGL(Handle: HDC): Boolean;
begin
 result:=False;
 if not LoadOpenGL then exit;

 SetPixelFormat(Handle);
 FHandle := Handle;

 FRC := wglCreateContext(Handle);
 if (FRC=0) then
 begin
   MessageDlg('Can''t create RC', mtError, [mbOk], 0);
   exit;
 end;

 if (not wglMakeCurrent(Handle, FRC)) then
 begin
   MessageDlg('Can''t activate RC', mtError, [mbOk], 0);
   exit;
 end;

 ClearExtensions;
 ReadExtensions;

 FInitialized:=True;
 FHandle:=Handle;
end;

procedure TGLEngine.SetupGL;
begin
 if (not FInitialized) then exit;

 glClearColor(76 / 255, 113 / 255, 166 / 255, 1.0);
 glEnable(gl_Cull_Face);

 glClearDepth(1.0); 	  // Enables Clearing Of The Depth Buffer
 glDepthFunc(GL_LESS);	  // The Type Of Depth Test To Do
 glEnable(GL_DEPTH_TEST); // Enables Depth Testing
 glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

 glMatrixMode(GL_PROJECTION);
 glEnable(GL_TEXTURE_2D);

 glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular[0]);
 glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess[0]);
 glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient[0]);
 glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse[0]);

 glLightfv(GL_LIGHT0, GL_POSITION, @light_position[0]);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);

 glLightModelfv(GL_LIGHT_MODEL_AMBIENT, @light_ambient[0]);
 glMatrixMode(GL_MODELVIEW);


end;


procedure TGLEngine.DeInit;
begin
  if FRC<>0 then
    begin
      if (not wglMakeCurrent(FHandle,0)) then
        MessageBox(0,'Release of DC and RC failed.',' Shutdown Error',MB_OK or MB_ICONERROR);
      if (not wglDeleteContext(FRC)) then
        begin
          MessageBox(0,'Release of Rendering Context failed.',' Shutdown Error',MB_OK or MB_ICONERROR);
        end
    end;
   FInitialized:=False;
end;

procedure TGLEngine.SetViewPort(X, Y, Width, Height: Integer);
begin
  glViewport(X, Y, Width, Height);
  FWidth := Width;  FHeight := Height;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, FWidth / FHeight, NearClipping, FarClipping);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity;
end;

procedure TGLEngine.SetPixelFormat(Handle: HDC);
  var
    PixelFormat      : TGLuint;
    PFD              : pixelformatdescriptor;
  begin
    with pfd do
      begin
        nSize:= SizeOf( PIXELFORMATDESCRIPTOR );
        nVersion:= 1;
        dwFlags:= PFD_DRAW_TO_WINDOW
          or PFD_SUPPORT_OPENGL
          or PFD_DOUBLEBUFFER;
        iPixelType:= PFD_TYPE_RGBA;
        cColorBits:= 16;
        cRedBits:= 0;
        cRedShift:= 0;
        cGreenBits:= 0;
        cBlueBits:= 0;
        cBlueShift:= 0;
        cAlphaBits:= 0;
        cAlphaShift:= 0;
        cAccumBits:= 0;
        cAccumRedBits:= 0;
        cAccumGreenBits:= 0;
        cAccumBlueBits:= 0;
        cAccumAlphaBits:= 0;
        cDepthBits:= 16;
        cStencilBits:= 0;
        cAuxBuffers:= 0;
        iLayerType:= PFD_MAIN_PLANE;
        bReserved:= 0;
        dwLayerMask:= 0;
        dwVisibleMask:= 0;
        dwDamageMask:= 0
      end;
    PixelFormat := ChoosePixelFormat(Handle, @pfd);
    if (PixelFormat=0) then
      MessageDlg('Can''t find a suitable PixelFormat.', mtError, [mbOk], 0);
    if (not Windows.SetPixelFormat(Handle,PixelFormat,@pfd)) then
      MessageDlg('Can''t set PixelFormat.', mtError, [mbOk], 0)
  end; (*SetPixelFormat*)

   procedure TGLEngine.ResetMaterial;
   begin
     glMaterialfv(GL_FRONT, GL_SPECULAR, @mat_specular[0]);
     glMaterialfv(GL_FRONT, GL_SHININESS, @mat_shininess[0]);
     glMaterialfv(GL_FRONT, GL_AMBIENT, @mat_ambient[0]);
     glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat_diffuse[0]);
   end;

   procedure TGLEngine.MakeCurrent (AHandle: HDC);
   begin
        if (not wglMakeCurrent(AHandle, FRC)) then
            raise Exception.Create('Can''t activate RC');
   end;

   procedure TGLEngine.SwapBuffers (AHandle: HDC);
   begin
       Windows.SwapBuffers(AHandle);
   end;

   procedure TGLEngine.SetClearColor (Color: TColor);
   begin
        glClearColor ((Color and $FF) / 255, ((Color shr 8) and $FF) / 255, ((Color shr 16) and $FF) / 255, 1.0);
   end;

    procedure SetColor (AColor: TColor);
    begin
        glColor4f ((AColor and $FF) / 255, ((AColor shr 8) and $FF) / 255, ((AColor shr 16) and $FF) / 255, 1.0);
    end;

    procedure SetLightColor (Color: TColor);
    var
       facs:  array[0..3] of TGLfloat ;
       mat: array[0..3] of TGLfloat;
       Index: Integer;
    begin
     facs[0] := (Color and $FF) / 255;
     facs[1] := ((Color shr 8) and $FF) / 255;
     facs[2] := ((Color shr 16) and $FF) / 255;
     facs[3] := 1.0;

     for Index := 0 to 3 do
         mat[Index] := mat_ambient[Index] * facs[Index];
     glMaterialfv(GL_FRONT, GL_AMBIENT, @mat[0]);
     for Index := 0 to 3 do
         mat[Index] := mat_diffuse[Index] * facs[Index];
     glMaterialfv(GL_FRONT, GL_DIFFUSE, @mat[0]);
    end;
end.
