unit Unit_Particles;
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

uses Unit_OpenGLGeometry, contnrs, Unit_OpenGL, SysUtils, Classes, Unit_Matrix, StrUtils, Unit_XMLNode,
pngimage;


type
  TAppearType = (atOnce, atPeriodical);

  TRandomValue = class (TObject)
      private
        FAverage: Single;
        FVariance: Single;
        function GetValue: Single;
      public
        property Average: Single read FAverage write FAverage;
        property Variance: Single read FVariance write FVariance;
        property Value: Single read GetValue;
        procedure LoadFromXMLNode (ANode: TXMLNode);
        procedure Save (Strings: TStrings; Spaces: Integer); // Only save content
        procedure Apply (ARandomValue: TRandomValue);
  end;


  TVectorCone = class (TObject) // This class holds all information about a
                                // random direction vector
      private
        FAngle1: Single; // Phi
        FAngle2: Single; // Theta
        FConeAngle: Single; // Alpha
        FAmount: TRandomValue;
      public
        property Angle1: Single read FAngle1 write FAngle1;
        property Angle2: Single read FAngle2 write FAngle2;
        property ConeAngle: Single read FConeAngle write FConeAngle;
        property Amount: TRandomValue read FAmount write FAmount;
        constructor Create;
        destructor Destroy; override;
        function CreateVector: TVector3f;
        procedure Save (Strings: TStrings; Spaces: Integer); // Only save content
        procedure LoadFromXMLNode (ANode: TXMLNode);
        procedure Apply (AVectorCone: TVectorCone);
  end;

  TParticleTypeDefinition = class(TObject)  // This class defines the type of a particle (Texture coords)
      private
         FID: Cardinal;
         FDescription: String;
         FTexX, FTexY, FWidth, FHeight: Integer;
      public
         property ID: Cardinal read FID;
         property Description: String read FDescription write FDescription;
         property TexX: Integer read FTexX write FTexX;
         property TexY: Integer read FTexY write FTexY;
         property Width: Integer read FWidth write FWidth;
         property Height: Integer read FHeight write FHeight;
         procedure Save (Strings: TStrings; Spaces: Integer);
         constructor CreateFromXMLNode (ANode: TXMLNode);
  end;


  TEmissionDefinition = class (TObject)
     private
        function GetName: String; virtual; abstract;
        function GetParameter: String; virtual; abstract;
     public
        property Name: String read GetName;
        property Parameter: String read GetParameter;
        function GetMatrix : TMMatrix; virtual; abstract;
        procedure Save (Strings: TStrings; Spaces: Integer); virtual; abstract;
  end;

  TEmission_Rotation = class (TEmissionDefinition)
     private
        FAngle1, FAngle2: Single;
        function GetName: String; override;
        function GetParameter: String; override;
     public
        property Angle1: Single read FAngle1 write FAngle1;
        property Angle2: Single read FAngle2 write FAngle2;
        constructor CreateFromXMLNode (ANode: TXMLNode);
        function GetMatrix : TMMatrix; override;
        procedure Save (Strings: TStrings; Spaces: Integer); override;
  end;

  TEmission_Circle = class (TEmissionDefinition)
     private
        FRadius: Single;
        function GetName: String; override;
        function GetParameter: String; override;
     public
        property Radius: Single read FRadius write FRadius;
        constructor Create;
        constructor CreateFromXMLNode (ANode: TXMLNode);
        function GetMatrix : TMMatrix; override;
        procedure Save (Strings: TStrings; Spaces: Integer); override;
  end;

  TEmission_Translation = class (TEmissionDefinition)
     private
        FX, FY, FZ: TRandomValue;
        function GetName: String; override;
        function GetParameter: String; override;
     public
        property X: TRandomValue read FX;
        property Y: TRandomValue read FY;
        property Z: TRandomValue read FZ;
        constructor Create;
        destructor Destroy; override;
        constructor CreateFromXMLNode (ANode: TXMLNode);
        function GetMatrix : TMMatrix; override;
        procedure Save (Strings: TStrings; Spaces: Integer); override;
  end;

  TEmission_Sphere = class (TEmissionDefinition)
     private
        FRadius: Single;
        function GetName: String; override;
        function GetParameter: String; override;
     public
        property Radius: Single read FRadius write FRadius;
        constructor Create;
        constructor CreateFromXMLNode (ANode: TXMLNode);
        function GetMatrix : TMMatrix; override;
        procedure Save (Strings: TStrings; Spaces: Integer); override;
  end;

  TParticleDefinition = class(TObject)  // This class contains a Definition of a Particle
     private
        FID: Cardinal;
        FDescription: String;
        FSpeed: TVectorCone;
        FAcceleration: TVectorCone;
        FLifeTime: TRandomValue;
        FAppearTime: TRandomValue;
        FSize: TRandomValue;
        FParticleType: Integer;
        FAmount: Integer;
        FFadeOut: Boolean;
        FFadeIn: Boolean;
        FTransformSpeed: Boolean;
        FTransformAcceleration: Boolean;
        FEmissionList: TObjectList;

        function GetEmission(Index: Integer): TEmissionDefinition;
        function GetEmissionCount: Integer;
     public
        property ID: Cardinal read FID;
        property Description: String read FDescription write FDescription;
        property Speed: TVectorCone read FSpeed;
        property Acceleration: TVectorCone read FAcceleration;
        property Size: TRandomValue read FSize;
        property LifeTime: TRandomValue read FLifeTime;
        property AppearTime: TRandomValue read FAppearTime;
        property ParticleType: Integer read FParticleType write FParticleType;
        property Amount: Integer read FAmount write FAmount;
        property FadeOut: Boolean read FFadeOut write FFadeOut;
        property FadeIn: Boolean read FFadeIn write FFadeIn;
        property TransformSpeed: Boolean read FTransformSpeed write FTransformSpeed;
        property TransformAcceleration: Boolean read FTransformAcceleration write FTransformAcceleration;

        property Emissions[Index: Integer]: TEmissionDefinition read GetEmission;
        property EmissionCount: Integer read GetEmissionCount;

        constructor Create(AID: Cardinal);
        constructor CreateFromXMLNode (ANode: TXMLNode);
        destructor Destroy; override;
        procedure Save(Strings: TStrings; Spaces: Integer);

        procedure AddEmission (AEmission: TEmissionDefinition);
        procedure RemoveEmission (AEmission: TEmissionDefinition);
        procedure MoveEmission (AEmission: TEmissionDefinition; Up: Boolean);

        procedure Apply (AParticle: TParticleDefinition);

  end;

  TEffectDefinition = class (TObject)
     private
        FName: String;
        FDescription: String;

        FTime: Cardinal;
        FAppearType: TAppearType;

        FParticles: TObjectList;
        FTypes: TObjectList;
        ParticleID: Cardinal;
        TypeID: Cardinal;

        FEffectID: Integer;

        FTextureFileName: String;
        FLockFlag: Boolean;

        function GetParticleArray(Index: Integer): TParticleDefinition;
        function GetTypeArray(Index: Integer): TParticleTypeDefinition;
        function GetParticleCount: Integer;
        function GetTypeCount: Integer;

        function GetAppearString: String;

     public
        property Description: String read FDescription write FDescription;
        property Name: String read FName write FName;
        property Time: Cardinal read FTime write FTime;
        property AppearType: TAppearType read FAppearType write FAppearType;
        property AppearString: String read GetAppearString;
        property EffectID: Integer read FEffectID write FEffectID;

        property Particles[Index: Integer]: TParticleDefinition read GetParticleArray;
        property Types[Index: Integer]: TParticleTypeDefinition read GetTypeArray;
        property ParticleCount: Integer read GetParticleCount;
        property TypeCount: Integer read GetTypeCount;

        property TextureFileName: String read FTextureFileName write FTextureFileName;
        property LockFlag: Boolean read FLockFlag;

        constructor Create;
        constructor CreateFromXMLNode (ANode: TXMLNode);
        constructor CreateFromStream (AStream: TStream; ACount: Integer);
        destructor Destroy; override;

        function AddParticle: TParticleDefinition;
        function AddType: TParticleTypeDefinition;
        function GetParticle(ID: Cardinal): TParticleDefinition;
        function GetType(ID: Cardinal): TParticleTypeDefinition;

        procedure Save(Strings: TStrings; Spaces: Integer);
        procedure SaveToFile (FileName: String);

        procedure RemoveType (ID: Cardinal);
        procedure RemoveParticle (ID: Cardinal);

        procedure Lock; // For multi-edit-issues
        procedure UnLock;
  end;

  TEffectCollection = class (TObject)
      private
        FEffects: TObjectList;
        function GetCount: Integer;
        function Get (Index: Integer): TEffectDefinition;
      public
        property Count: Integer read GetCount;
        property Items[Index: Integer]: TEffectDefinition read Get;

        constructor Create;
        constructor CreateFromXMLNode (ANode: TXMLNode);
        constructor CreateFromStream (AStream: TStream; ACount: Integer);

        destructor Destroy; override;

        procedure AddEffect (AEffect: TEffectDefinition);
        procedure RemoveEffect (AEffect: TEffectDefinition);

        procedure Save(Strings: TStrings; Spaces: Integer);
        procedure SaveToFile (FileName: String);
  end;



  TParticle = class(TObject)
    private
       vec_pos: TVector3f;
       vec_speed: TVector3f;
       vec_acc: TVector3f;
       m_lifetime: Single;
       m_time: Single;
       m_fadeOut: Boolean;
       m_fadeIn: Boolean;
       m_SizeFactor: Single; // 1 = 100%
       texcoords: array[0..1, 0..1] of Single;
    public
       constructor Create(Position: TVector3f; LifeTime: Single);
       destructor Destroy; override;
       procedure Handle (time_fac: Single); virtual;
       procedure Draw (vec1, vec2: TVertex); virtual;
       procedure Accelerate (vec: TVertex); virtual;
       procedure SetAcceleration (vec: TVertex); virtual;
       procedure SetTexCoords(x, y, w, h: Single);
       procedure SetSize (AFactor: Single);
  end;

  TEffect = class (TObject)
       private
           FActive: Boolean;
           FStartTime: Cardinal;
           FNextEmission: Cardinal;
           FDefinition: TEffectDefinition;
           X, Y, Z: Single;
       public
           constructor Create(ADefinition: TEffectDefinition);
           procedure Start;
           procedure Stop;
  end;

  TParticleHandler = class(TObject)
      private
         particles: TObjectList;
         effects: TObjectList;
         last_handle: Cardinal;
         Texture: TGLuint;
         FTextureWidth, FTextureHeight: Integer;
         function GetEffectCount: Integer;
         function GetParticleCount: Integer;
      public
         property EffectCount: Integer read GetEffectCount;
         property ParticleCount: Integer read GetParticleCount;
         constructor Create;
         destructor Destroy; override;
         procedure Handle;
         procedure ClearParticles;
         procedure AddParticle(particle: TParticle);
         function AddEffect(Definition: TEffectDefinition; X, Y, Z: Single): TEffect;
         procedure Render;
         procedure Start;
         procedure LoadTexture(APNG: TPNGObject);
  end;


implementation

const SpaceString: String = #9;

function Rnd (X: Double): Double;
begin
  result := X * random(100000) / 100000.0;
end;

procedure TRandomValue.LoadFromXMLNode (ANode: TXMLNode);
begin
    if not (Assigned (ANode)) then exit;
    FAverage := StrToFloat(ANode.GetChildValue('average'));
    FVariance := StrToFloat(ANode.GetChildValue('variance'));
end;

function TRandomValue.GetValue: Single;
begin
    result := FAverage + (random(100000) / 50000.0 - 1.0) * FVariance;
end;

procedure TRandomValue.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));
  Strings.Add(Prefix + Format('<average>%.4f</average>', [ FAverage ]));
  Strings.Add(Prefix + Format('<variance>%.4f</variance>', [ FVariance ]));
end;

procedure TRandomValue.Apply (ARandomValue: TRandomValue);
begin
  Assert (Assigned (ARandomValue));
  FAverage := ARandomValue.FAverage;
  FVariance := ARandomValue.FVariance;
end;

procedure TVectorCone.LoadFromXMLNode (ANode: TXMLNode);
begin
    if not (Assigned (ANode)) then exit;
    FAngle1 := StrToFloat(ANode.GetChildValue('angle1'));
    FAngle2 := StrToFloat(ANode.GetChildValue('angle2'));
    FConeAngle := StrToFloat(ANode.GetChildValue('coneangle'));
    FAmount.LoadFromXMLNode(ANode.FindChild('amount', false));
end;

constructor TVectorCone.Create;
begin
    FAmount := TRandomValue.Create;
end;

destructor TVectorCone.Destroy;
begin
    FAmount.Free;
end;

function TVectorCone.CreateVector: TVector3f;
var
  Phi, Theta, Alpha, Psi, Zeta: Single;
  TempDir: TVector3f;
  Matrix: TMMatrix;
  MVector, ResultVector, Axis: TMVector;

begin
    Phi := FAngle1 * 3.14159 / 180;
    Theta := FAngle2 * 3.14159 / 180;

    alpha := FConeAngle;
    Psi := Rnd (360) * 3.14159 / 180;
//    Zeta := {(Rnd (alpha))}ArcCos (1 - 2 * Rnd(cos ((180 - alpha) * 3.14159 / 180 / 2)));//
    Zeta := (Rnd (alpha))  * 3.14159 / 180;

    TempDir[0] := sin (Zeta) * cos (Psi);
    TempDir[1] := sin (Zeta) * sin (Psi);
    TempDir[2] := cos (Zeta);

    if Theta = 0 then
        result := TempDir
    else begin
        Axis := TMVector.Create(VectorCrossProduct (AffineVectorMake (sin (Theta) * cos (Phi), sin (Theta) * sin (Phi), cos (Theta)), AffineVectorMake (0, 0, 1)));
        try
             Matrix := TMMatrix.CreateRotationMatrix(Axis, Theta);
             try
               MVector := TMVector.Create(TempDir);
               try
                  ResultVector := Matrix.Apply(MVector);
                  try
                     result[0] := ResultVector[0];
                     result[1] := ResultVector[1];
                     result[2] := ResultVector[2];
                  finally
                     ResultVector.Free;
                  end;

               finally
                  MVector.Free;
               end;
             finally
                Matrix.Free;
             end;

         finally
            Axis.Free;
         end;
     end;

     ScaleVector (result, FAmount.Value);
end;

procedure TVectorCone.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));
  Strings.Add(Prefix + Format('<angle1>%.4f</angle1>', [ FAngle1 ]));
  Strings.Add(Prefix + Format('<angle2>%.4f</angle2>', [ FAngle2 ]));
  Strings.Add(Prefix + Format('<coneangle>%.4f</coneangle>', [ FConeAngle ]));
  Strings.Add(Prefix + '<amount>');
  FAmount.Save (Strings, Spaces + 1);
  Strings.Add(Prefix + '</amount>');
end;

procedure TVectorCone.Apply (AVectorCone: TVectorCone);
begin
  Assert (Assigned (AVectorCone));
  FAngle1 := AVectorCone.FAngle1;
  FAngle2 := AVectorCone.FAngle2;
  FConeAngle := AVectorCone.FConeAngle;
  FAmount.Apply(AVectorCone.Amount);
end;

constructor TParticleTypeDefinition.CreateFromXMLNode (ANode: TXMLNode);
begin
    Assert(Assigned (ANode));
    FID := StrToIntDef(ANode.GetChildValue('id'), 0);
    FDescription := ANode.GetChildValue('description');
    FTexX := StrToIntDef(ANode.GetChildValue('texcoord_x'), 0);
    FTexY := StrToIntDef(ANode.GetChildValue('texcoord_y'), 0);
    FWidth := StrToIntDef(ANode.GetChildValue('texcoord_w'), 0);
    FHeight := StrToIntDef(ANode.GetChildValue('texcoord_h'), 0);
end;

procedure TParticleTypeDefinition.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
    Assert (Assigned(Strings));
    Prefix := DupeString (SpaceString, Spaces);
    Strings.Add (Prefix + Format('<id>%d</id>', [ FID ]));
    Strings.Add (Prefix + Format('<description>%s</description>', [ FDescription ]));
    Strings.Add (Prefix + Format('<texcoord_x>%d</texcoord_x>', [ FTexX ]));
    Strings.Add (Prefix + Format('<texcoord_y>%d</texcoord_y>', [ FTexy ]));
    Strings.Add (Prefix + Format('<texcoord_w>%d</texcoord_w>', [ FWidth ]));
    Strings.Add (Prefix + Format('<texcoord_h>%d</texcoord_h>', [ FHeight ]));
end;

constructor TEmission_Rotation.CreateFromXMLNode (ANode: TXMLNode);
begin
  Assert (Assigned (ANode));
  FAngle1 := StrToFloat(ANode.GetChildValue('angle1'));
  FAngle2 := StrToFloat(ANode.GetChildValue('angle2'));
end;


function TEmission_Rotation.GetName: String;
begin
  result := 'Rotation';
end;

function TEmission_Rotation.GetParameter: String;
begin
  result := Format ('%.1f° / %.1f°', [ FAngle1, FAngle2 ]);
end;

procedure TEmission_Rotation.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));

  Strings.Add(Prefix + '<type>rotation</type>');
  Strings.Add(Prefix + Format('<angle1>%.4f</angle1>', [ FAngle1 ]));
  Strings.Add(Prefix + Format('<angle2>%.4f</angle2>', [ FAngle2 ]));
end;

function TEmission_Rotation.GetMatrix : TMMatrix;
var
  Axis: TMVector;
  Phi, Theta: Single;
begin
    Phi := FAngle1 * 3.14159 / 180;
    Theta := FAngle2 * 3.14159 / 180;

    Axis := TMVector.Create(VectorCrossProduct (AffineVectorMake (sin (Theta) * cos (Phi), sin (Theta) * sin (Phi), cos (Theta)), AffineVectorMake (0, 0, 1)));
    try
        if (Axis.X = 0) and (Axis.Y = 0) and (Axis.Z = 0) then
            result := TMMatrix.Create // Identity Matrix
        else
           result := TMMatrix.CreateRotationMatrix(Axis, Theta);
    finally
       Axis.Free;
    end;
end;

constructor TEmission_Translation.Create;
begin
    FX := TRandomValue.Create;
    FY := TRandomValue.Create;
    FZ := TRandomValue.Create;
end;

destructor TEmission_Translation.Destroy;
begin
    FX.Free;
    FY.Free;
    FZ.Free;
end;

constructor TEmission_Translation.CreateFromXMLNode (ANode: TXMLNode);
begin
  Assert (Assigned (ANode));
  Create;
  FX.LoadFromXMLNode(ANode.FindChild('x'));
  FY.LoadFromXMLNode(ANode.FindChild('y'));
  FZ.LoadFromXMLNode(ANode.FindChild('z'));
end;


function TEmission_Translation.GetName: String;
begin
  result := 'Translation';
end;

function TEmission_Translation.GetParameter: String;
begin
  result := Format ('%.2f / %.2f / %.2f', [ FX.Average, FY.Average, FZ.Average ]);
end;

procedure TEmission_Translation.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));

  Strings.Add(Prefix + '<type>translation</type>');
  Strings.Add(Prefix + '<x>');
  FX.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</x>');

  Strings.Add(Prefix + '<y>');
  FY.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</y>');

  Strings.Add(Prefix + '<z>');
  FZ.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</z>');
end;

function TEmission_Translation.GetMatrix : TMMatrix;
var
  Vector: TMVector;
begin
    Vector := TMVector.Create(FX.Value, FY.Value, FZ.Value);
    try
       result := TMMatrix.CreateTranslationMatrix(Vector);
    finally
       Vector.Free;
    end;
end;

constructor TEmission_Circle.Create;
begin
  FRadius := 0.2;
end;

constructor TEmission_Circle.CreateFromXMLNode (ANode: TXMLNode);
begin
  Assert (Assigned (ANode));
  Create;
  FRadius := StrToFloat(ANode.GetChildValue('radius'));
end;

function TEmission_Circle.GetName: String;
begin
  result := 'Circle';
end;

function TEmission_Circle.GetParameter: String;
begin
  result := Format ('Radius: %.2f', [ FRadius ]);
end;

procedure TEmission_Circle.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));

  Strings.Add(Prefix + '<type>circle</type>');
  Strings.Add(Prefix + Format('<radius>%.4f</radius>', [ FRadius ]));
end;

function TEmission_Circle.GetMatrix : TMMatrix;
var
  R, Phi: Single;
  Vector: TMVector;
begin
    R := Rnd (FRadius);
    Phi := rnd (360) * 3.14159 / 180;

    Vector := TMVector.Create(R * cos (Phi), R * Sin (Phi), 0);
    try
       result := TMMatrix.CreateTranslationMatrix(Vector);
    finally
       Vector.Free;
    end;
end;

constructor TEmission_Sphere.Create;
begin
  FRadius := 0.5;
end;

constructor TEmission_Sphere.CreateFromXMLNode (ANode: TXMLNode);
begin
  Assert (Assigned (ANode));
  Create;
  FRadius := StrToFloat(ANode.GetChildValue('radius'));
end;

function TEmission_Sphere.GetName: String;
begin
  result := 'Sphere';
end;

function TEmission_Sphere.GetParameter: String;
begin
  result := Format ('Radius: %.2f', [ FRadius ]);
end;

procedure TEmission_Sphere.Save (Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));

  Strings.Add(Prefix + '<type>sphere</type>');
  Strings.Add(Prefix + Format('<radius>%.4f</radius>', [ FRadius ]));
end;

function TEmission_Sphere.GetMatrix : TMMatrix;
var
  R, Phi, Theta: Single;
  Vector: TMVector;
begin
    R := Rnd (FRadius);
    Phi := rnd (360) * 3.14159 / 180;
    Theta := (Rnd (180))  * 3.14159 / 180;

    Vector := TMVector.Create(R * sin (Theta) * cos (Phi), R * Sin (Theta) * sin (Phi), R * cos (Theta));
    try
       result := TMMatrix.CreateTranslationMatrix(Vector);
    finally
       Vector.Free;
    end;
end;

constructor TParticleDefinition.Create(AID: Cardinal);
begin
     FEmissionList := TObjectList.Create;
     FSpeed := TVectorCone.Create;
     FAcceleration := TVectorCone.Create;
     FLifeTime := TRandomValue.Create;
     FSize := TRandomValue.Create;
     FAppearTime := TRandomValue.Create;
     FID := AID;
     FDescription := 'New Particle';
     FLifeTime.Average := 5;
     FSize.Average := 100;
     FAmount := 1;
     FFadeOut := True;
     FTransformSpeed := True;
     FTransformAcceleration := True;
end;



constructor TParticleDefinition.CreateFromXMLNode (ANode: TXMLNode);
var
  Index: Integer;
  S: String;
begin
    Assert (Assigned (ANode));
    Create (StrToIntDef(ANode.GetChildValue('id'), 0));
    FDescription := ANode.GetChildValue('description');
    FAmount := StrToIntDef(ANode.GetChildValue('amount'), 0);
    FParticleType := StrToIntDef(ANode.GetChildValue('particletype'), 0);
    FSpeed.LoadFromXMLNode(ANode.FindChild('speed', false));
    FAcceleration.LoadFromXMLNode(ANode.FindChild('acceleration', false));
    FLifeTime.LoadFromXMLNode(ANode.FindChild('lifetime', false));
    FAppearTime.LoadFromXMLNode(ANode.FindChild('appeartime', false));
    FSize.LoadFromXMLNode(ANode.FindChild('size', false));
    FFadeOut := StrToIntDef(ANode.GetChildValue('fadeout'), 0) <> 0;
    FFadeIn := StrToIntDef(ANode.GetChildValue('fadein'), 0) <> 0;
    FTransformSpeed := StrToIntDef(ANode.GetChildValue('transform_speed'), 0) <> 0;
    FTransformAcceleration := StrToIntDef(ANode.GetChildValue('transform_acceleration'), 0) <> 0;

    For Index := 0 to ANode.ChildCount - 1 do
      if ANode.Children[Index].Section = 'emission' then begin
          S := ANode.Children[Index].GetChildValue('type');
          if S = 'rotation' then
              FEmissionList.Add(TEmission_Rotation.CreateFromXMLNode(ANode.Children[Index]));
          if S = 'translation' then
              FEmissionList.Add(TEmission_Translation.CreateFromXMLNode(ANode.Children[Index]));
          if S = 'circle' then
              FEmissionList.Add(TEmission_Circle.CreateFromXMLNode(ANode.Children[Index]));
          if S = 'sphere' then
              FEmissionList.Add(TEmission_Sphere.CreateFromXMLNode(ANode.Children[Index]));
      end;
end;

destructor TParticleDefinition.Destroy;
begin
     FSpeed.Free;
     FAcceleration.Free;
     FLifeTime.Free;
     FSize.Free;
     FAppearTime.Free;
     FEmissionList.Free;
end;

procedure TParticleDefinition.Save(Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
  Index: Integer;
begin
  Prefix := DupeString (SpaceString, Spaces);
  Assert (Assigned (Strings));

  Strings.Add(Prefix + Format('<id>%d</id>', [ FID ]));
  Strings.Add(Prefix + Format('<description>%s</description>', [ FDescription ]));
  Strings.Add(Prefix + Format('<amount>%d</amount>', [ FAmount ]));
  Strings.Add(Prefix + Format('<particletype>%d</particletype>', [ FParticleType ]));
  Strings.Add(Prefix + Format('<fadeout>%d</fadeout>', [ Ord (FFadeOut) ]));
  Strings.Add(Prefix + Format('<fadein>%d</fadein>', [ Ord (FFadeIn) ]));
  Strings.Add(Prefix + Format('<transform_speed>%d</transform_speed>', [ Ord (FTransformSpeed) ]));
  Strings.Add(Prefix + Format('<transform_acceleration>%d</transform_acceleration>', [ Ord (FTransformAcceleration) ]));

  Strings.Add(Prefix + '<speed>');
  FSpeed.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</speed>');

  Strings.Add(Prefix + '<acceleration>');
  FAcceleration.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</acceleration>');

  Strings.Add(Prefix + '<lifetime>');
  FLifeTime.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</lifetime>');

  Strings.Add(Prefix + '<size>');
  FSize.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</size>');

  Strings.Add(Prefix + '<appeartime>');
  FAppearTime.Save(Strings, Spaces + 1);
  Strings.Add(Prefix + '</appeartime>');

  For Index := 0 to EmissionCount - 1 do begin
      Strings.Add(Prefix + '<emission>');
      Emissions[Index].Save(Strings, Spaces + 1);
      Strings.Add(Prefix + '</emission>');
  end;

end;

function TParticleDefinition.GetEmission(Index: Integer): TEmissionDefinition;
begin
     result := FEmissionList[Index] as TEmissionDefinition;
end;

function TParticleDefinition.GetEmissionCount: Integer;
begin
     result := FEmissionList.Count;
end;

procedure TParticleDefinition.AddEmission (AEmission: TEmissionDefinition);
begin
  Assert (Assigned (AEmission));
  FEmissionList.Add(AEmission);
end;

procedure TParticleDefinition.RemoveEmission (AEmission: TEmissionDefinition);
var
  Index: Integer;
begin
  For Index := 0 to FEmissionList.Count - 1 do
      if FEmissionList[Index] = AEmission then begin
          FEmissionList.Delete(Index);
          break;
      end;
end;

procedure TParticleDefinition.MoveEmission (AEmission: TEmissionDefinition; Up: Boolean);
var
  Index, NewIndex: Integer;
begin
  For Index := 0 to FEmissionList.Count - 1 do
      if FEmissionList[Index] = AEmission then begin
          if Up then
              NewIndex := Index - 1
          else
              NewIndex := Index + 1;
          if (NewIndex >= 0) and (NewIndex < FEmissionList.Count) then // Range Check
              FEmissionList.Move(Index, NewIndex);
          break;
      end;
end;

procedure TParticleDefinition.Apply (AParticle: TParticleDefinition);
begin
   Assert (Assigned (AParticle));
   FDescription := AParticle.FDescription;
   FSpeed.Apply (AParticle.FSpeed);
   FAcceleration.Apply (AParticle.FAcceleration);
   FLifeTime.Apply (AParticle.FLifeTime);
   FAppearTime.Apply (AParticle.FAppearTime);
   FSize.Apply (AParticle.FSize);
   FParticleType := AParticle.FParticleType;
   FAmount := AParticle.FAmount;
   FFadeOut := AParticle.FFadeOut;
   FFadeIn := AParticle.FFadeIn;
   FTransformSpeed := AParticle.FTransformSpeed;
   FTransformAcceleration := AParticle.FTransformAcceleration;
end;


constructor TEffectDefinition.Create;
begin
     FDescription := 'New Effect';
     FAppearType := atPeriodical;
     FTime := 100;

     FParticles := TObjectList.Create;
     FTypes := TObjectList.Create;
     ParticleID := 1;
     TypeID := 1;
end;

constructor TEffectDefinition.CreateFromXMLNode (ANode: TXMLNode);
var
  Index: Integer;
  Node: TXMLNode;
begin
    Assert (Assigned (ANode));

    if ANode.Section <> 'effect' then
        raise Exception.Create ('Node is not an effect definition');

    Create;

    FTextureFileName := ANode.GetChildValue('texture');
    FDescription := ANode.GetChildValue('description');
    FName := ANode.GetChildValue('name');
    FEffectID := StrToIntDef(ANode.GetChildValue('effect_id'), 0);
    FAppearType := TAppearType (StrToIntDef(ANode.GetChildValue('appeartype'), 0));
    FTime := StrToIntDef(ANode.GetChildValue('time'), 0);

    For Index := 0 to ANode.ChildCount - 1 do begin
        Node := ANode.Children[Index];
        if Node.Section = 'particletype_definition' then
            FTypes.Add(TParticleTypeDefinition.CreateFromXMLNode(Node));
        if Node.Section = 'particle_definition' then
            FParticles.Add(TParticleDefinition.CreateFromXMLNode(Node));
    end;

    For Index := 0 to TypeCount - 1 do
        if Types[Index].ID >= TypeID then
            TypeID := Types[Index].ID + 1;

    For Index := 0 to ParticleCount - 1 do
        if Particles[Index].ID >= ParticleID then
            ParticleID := Particles[Index].ID + 1;
end;

constructor TEffectDefinition.CreateFromStream (AStream: TStream; ACount: Integer);
var
  XMLFile: TXMLFile;
begin
     XMLFile := TXMLFile.Create (AStream, ACount);
     try
        if not Assigned (XMLFile.RootNode) then
            raise Exception.Create ('Effect Node not found');

        CreateFromXMLNode (XMLFile.RootNode);

     finally
         XMLFile.Free;
     end;
end;

destructor TEffectDefinition.Destroy;
begin
     FParticles.Free;
     FParticles := nil;
     FTypes.Free;
     FTypes := nil;
end;

procedure TEffectDefinition.Save(Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
  Index: Integer;
begin
    Assert (Assigned(Strings));
    Prefix := DupeString (SpaceString, Spaces);
    Strings.Add (Prefix + '<texture>'+FTextureFileName+'</texture>');
    Strings.Add (Prefix + '<description>'+FDescription+'</description>');
    Strings.Add (Prefix + '<name>'+FName+'</name>');
    Strings.Add (Prefix + '<appeartype>'+IntToStr(Ord(FAppearType))+'</appeartype>');
    Strings.Add (Prefix + '<time>'+IntToStr(FTime)+'</time>');
    Strings.Add (Prefix + '<effect_id>'+IntToStr(FEffectID)+'</effect_id>');

    For Index := 0 to TypeCount - 1 do begin
        Strings.Add (Prefix + '<particletype_definition>');
        Types[Index].Save (Strings, Spaces + 1);
        Strings.Add (Prefix + '</particletype_definition>');
        Strings.Add ('');
    end;

    For Index := 0 to ParticleCount - 1 do begin
        Strings.Add (Prefix + '<particle_definition>');
        Particles[Index].Save (Strings, Spaces + 1);
        Strings.Add (Prefix + '</particle_definition>');
        Strings.Add ('');
    end;
end;


function TEffectDefinition.AddParticle: TParticleDefinition;
begin
     result := TParticleDefinition.Create(ParticleID);
     FParticles.Add(result);
     inc(ParticleID);
end;

function TEffectDefinition.AddType: TParticleTypeDefinition;
begin
     result := TParticleTypeDefinition.Create;
     result.FID := TypeID;
     FTypes.Add(result);
     inc(TypeID);
end;

function TEffectDefinition.GetParticleArray(Index: Integer): TParticleDefinition;
begin
     result := FParticles[Index] as TParticleDefinition;
end;

function TEffectDefinition.GetTypeArray(Index: Integer): TParticleTypeDefinition;
begin
     result := FTypes[Index] as TParticleTypeDefinition;
end;

function TEffectDefinition.GetParticleCount: Integer;
begin
     result := FParticles.Count;
end;

function TEffectDefinition.GetTypeCount: Integer;
begin
     result := FTypes.Count;
end;

procedure TEffectDefinition.RemoveType (ID: Cardinal);
var
   Index: Integer;
begin
     For Index := TypeCount - 1 downto 0 do
         if Types[Index].ID = ID then
            FTypes.Delete(Index);
end;

procedure TEffectDefinition.RemoveParticle (ID: Cardinal);
var
   Index: Integer;
begin
     For Index := ParticleCount - 1 downto 0 do
         if Particles[Index].ID = ID then
            FParticles.Delete(Index);
end;

function TEffectDefinition.GetParticle(ID: Cardinal): TParticleDefinition;
var
   Index: Integer;
begin
     For Index := 0 to ParticleCount - 1 do
         if Particles[Index].ID = ID then begin
            result := Particles[Index];
            exit;
         end;
     result := nil;
end;

function TEffectDefinition.GetType(ID: Cardinal): TParticleTypeDefinition;
var
   Index: Integer;
begin
     For Index := 0 to TypeCount - 1 do
         if Types[Index].ID = ID then begin
            result := Types[Index];
            exit;
         end;
     result := nil;
end;

procedure TEffectDefinition.SaveToFile (FileName: String);
var
  Strings: TStrings;
begin
   Strings := TStringList.Create;
   try
      Strings.Add ('<?xml version="1.0" encoding="ISO-8859-1"?>');
      Strings.Add ('<effect>');
      Save (Strings, 1);
      Strings.Add ('</effect>');
      Strings.SaveToFile(FileName);
   finally
      Strings.Free;
   end;

end;

function TEffectDefinition.GetAppearString: String;
begin
  if FAppearType = atOnce then
      result := Format ('after %d ms', [FTime])
  else            
      result := Format ('every %d ms', [FTime]);
end;

procedure TEffectDefinition.Lock;
begin
    if FLockFlag then
        raise Exception.Create ('Effect "' + FName + '" is currently edited!');
    FLockFlag := True;
end;

procedure TEffectDefinition.UnLock;
begin
  FLockFlag := False;
end;

constructor TEffectCollection.Create;
begin
    FEffects := TObjectList.Create;
end;

destructor TEffectCollection.Destroy;
begin
    FEffects.Free;
end;

function TEffectCollection.GetCount: Integer;
begin
    result := FEffects.Count;
end;

function TEffectCollection.Get (Index: Integer): TEffectDefinition;
begin
    result := FEffects [Index] as TEffectDefinition;
end;

constructor TEffectCollection.CreateFromXMLNode (ANode: TXMLNode);
var
  Index: Integer;
  Node: TXMLNode;
begin
    Assert (Assigned (ANode));

    if ANode.Section <> 'effect_collection' then
        raise Exception.Create ('Node is not an effect definition');

    Create;

    For Index := 0 to ANode.ChildCount - 1 do begin
        Node := ANode.Children[Index];
        if Node.Section = 'effect' then
            FEffects.Add(TEffectDefinition.CreateFromXMLNode(Node));
    end;
end;

constructor TEffectCollection.CreateFromStream (AStream: TStream; ACount: Integer);
var
  XMLFile: TXMLFile;
begin
     XMLFile := TXMLFile.Create (AStream, ACount);
     try
        if not Assigned (XMLFile.RootNode) then
            raise Exception.Create ('Collection Node not found');

        CreateFromXMLNode (XMLFile.RootNode);

     finally
         XMLFile.Free;
     end;
end;

procedure TEffectCollection.AddEffect (AEffect: TEffectDefinition);
begin
  Assert (Assigned (AEffect));
  FEffects.Add(AEffect);
end;

procedure TEffectCollection.RemoveEffect (AEffect: TEffectDefinition);
var
  Index: Integer;
begin
  For Index := 0 to FEffects.Count - 1 do
      if FEffects[Index] = AEffect then begin
          FEffects.Delete(Index);
          break;
      end;
end;

procedure TEffectCollection.Save(Strings: TStrings; Spaces: Integer);
var
  Prefix: String;
  Index: Integer;
begin
    Assert (Assigned(Strings));
    Prefix := DupeString (SpaceString, Spaces);

    For Index := 0 to Count - 1 do begin
        Strings.Add (Prefix + '<effect>');
{flo
        Strings.Add(Prefix + Format('<effect_id>%d</effect_id>', [ Index ])); }
        Items[Index].Save (Strings, Spaces + 1);
        Strings.Add (Prefix + '</effect>');
        Strings.Add ('');
    end;
end;

procedure TEffectCollection.SaveToFile (FileName: String);
var
  Strings: TStrings;
begin
   Strings := TStringList.Create;
   try
      Strings.Add ('<?xml version="1.0" encoding="ISO-8859-1"?>');
      Strings.Add ('<effect_collection>');
      Save (Strings, 1);
      Strings.Add ('</effect_collection>');
      Strings.SaveToFile(FileName);
   finally
      Strings.Free;
   end;

end;

constructor TParticle.Create(Position: TVertex; LifeTime: Single);
var
   I: Integer;
begin
     vec_pos := Position;
     m_LifeTime := LifeTime;
     m_Time := LifeTime;
     m_SizeFactor := 1;
     For I := 0 to 2 do begin
         vec_speed[I] := 0;
         vec_acc[I] := 0;
     end;
     texcoords[0,0] := 0;
     texcoords[0,1] := 0;
     texcoords[1,0] := 1;
     texcoords[1,1] := 1;
     m_FadeOut := True;

end;

destructor TParticle.Destroy;
begin
end;

procedure TParticle.Handle (time_fac: Single);
var
   Index: Integer;
begin
   if (m_Time > 0) and (m_Time <= m_LifeTime) then begin
     For Index := 0 to 2 do
         vec_pos[Index] := vec_pos[Index] + vec_speed[Index] * time_fac;
     For Index := 0 to 2 do
         vec_speed[Index] := vec_speed[Index] + vec_acc[Index] * time_fac;
   end;
   if (m_Time > 0) then
     m_Time := m_Time - time_fac
end;

procedure TParticle.Draw (vec1, vec2: TVertex);
var
   i: Integer;
   vec: array[0..3, 0..2] of Single;
begin
     if (m_time < 0) or (m_time > m_lifetime) then exit;
	for i := 0 to 2 do begin
		vec[0,i] := vec_pos[i] - 0.5 * (vec1[i] + vec2[i]) * m_SizeFactor;
		vec[1,i] := vec[0,i] + vec1[i] * m_SizeFactor;
		vec[2,i] := vec[1,i] + vec2[i] * m_SizeFactor;
		vec[3,i] := vec[0,i] + vec2[i] * m_SizeFactor;
	end;
        if m_FadeOut then
           glColor4f(1.0,1.0,1.0,m_time * m_time /m_lifetime / m_lifetime)
        else
      	   glColor4f(1.0,1.0,1.0,1.0);
	glTexCoord2f(texcoords[0,0], texcoords[0,1]);
	glVertex3fv(@vec[0,0]);
	glTexCoord2f(texcoords[1,0], texcoords[0,1]);
	glVertex3fv(@vec[1,0]);
	glTexCoord2f(texcoords[1,0], texcoords[1,1]);
	glVertex3fv(@vec[2,0]);
	glTexCoord2f(texcoords[0,0], texcoords[1,1]);
	glVertex3fv(@vec[3,0]);
end;

procedure TParticle.Accelerate (vec: TVertex);
var
   Index: Integer;
begin
     For Index := 0 to 2 do
         vec_speed[Index] := vec_speed[Index] + vec[Index];
end;

procedure TParticle.SetAcceleration (vec: TVertex);
begin
     vec_acc := vec;
end;

procedure TParticle.SetTexCoords(x, y, w, h: Single);
begin
	texcoords[0,0] := x;
	texcoords[0,1] := y;
	texcoords[1,0] := x + w;
	texcoords[1,1] := y + h;
end;

procedure TParticle.SetSize (AFactor: Single);
begin
  m_SizeFactor := AFactor;
end;

constructor TEffect.Create(ADefinition: TEffectDefinition);
begin
     FDefinition := ADefinition;
     Start;
end;

procedure TEffect.Start;
begin
     FActive := True;
     FStartTime := Trunc(Time * 24 * 3600 * 1000);
     FNextEmission := FStartTime + FDefinition.FTime;
end;

procedure TEffect.Stop;
begin
     FActive := False;
end;

constructor TParticleHandler.Create;
begin
     Particles := TObjectList.Create;
     Effects := TObjectList.Create;
     FTextureWidth := 256;
     FTextureHeight := 256;
end;

destructor TParticleHandler.Destroy;
begin
     Particles.Free;
     Effects.Free;
end;


procedure TParticleHandler.Handle;
var
   CurrentTime: Cardinal;
   time_fac: Single;
   Index, PIndex, VIndex, EIndex: Integer;
   Particle: TParticle;
   Effect: TEffect;
   ParticleDefinition: TParticleDefinition;
   ParticleType: TParticleTypeDefinition;
   V: TVertex;
   Matrix, TmpMatrix: TMMatrix;

begin
  glDisable(GL_LIGHTING);
     CurrentTime := Trunc(Time * 24 * 3600 * 1000);
     time_fac := (currenttime - last_handle) * 0.001;
     if (time_fac <= 0.0) then
		time_fac := 0.001;
     if (time_fac >= 1.0) then
		time_fac := 1.0;
     last_handle := currenttime;
     For Index := Particles.Count - 1 downto 0 do begin
         Particle := Particles[Index] as TParticle;
         Particle.Handle(time_fac);
         if (Particle.m_Time < 0) then
            Particles.Delete(Index);
     end;

     For Index := 0 to Effects.Count - 1 do begin
        Effect := Effects[Index] as TEffect;
         if (Effect.FActive) and (Effect.FNextEmission <= CurrentTime) then begin
            with Effect do begin
                  For PIndex := 0 to FDefinition.ParticleCount - 1 do begin
                      ParticleDefinition := FDefinition.Particles[PIndex];
                      For VIndex := 0 to ParticleDefinition.Amount - 1 do begin

                         Matrix := TMMatrix.Create;
                         try
                            For EIndex := 0 to ParticleDefinition.EmissionCount - 1 do begin
                                TmpMatrix := ParticleDefinition.Emissions[EIndex].GetMatrix;
                                try
                                    Matrix.MultiplyRight(TmpMatrix);
                                finally
                                    TmpMatrix.Free;
                                end;
                            end;

                            V := AffineVectorMake (0, 0, 0);
                            Matrix.Apply(V[0], V[1], V[2]);
                            Particle := TParticle.Create(V, ParticleDefinition.LifeTime.Value);

                            V := ParticleDefinition.Speed.CreateVector;
                            if ParticleDefinition.FTransformSpeed then
                                Matrix.ApplyRotation(V[0], V[1], V[2]);
                            Particle.Accelerate (V);

                            V := ParticleDefinition.Acceleration.CreateVector;
                            if ParticleDefinition.FTransformAcceleration then
                                Matrix.ApplyRotation(V[0], V[1], V[2]);
                            Particle.SetAcceleration(V);

                            Particle.SetSize(ParticleDefinition.FSize.Value / 100); // Convert percent to a factor
                            Particle.m_time := Particle.m_time + ParticleDefinition.AppearTime.Value  / 1000;
                            Particle.m_fadeOut := ParticleDefinition.FFadeOut;
                            Particle.m_fadeIn := ParticleDefinition.FFadeIn;

                            ParticleType := FDefinition.GetType (ParticleDefinition.FParticleType);
                            if Assigned(ParticleType) then
                               Particle.SetTexCoords(ParticleType.TexX / FTextureWidth, ParticleType.TexY / FTextureHeight, ParticleType.Width / FTextureWidth, ParticleType.Height / FTextureHeight);

                            Particles.Add(Particle);
                         finally
                            Matrix.Free;
                         end;



                      end;
                  end;
            end;

            if Effect.FDefinition.AppearType = atPeriodical then
               inc(Effect.FNextEmission, Effect.FDefinition.Time)
            else
               Effect.Stop;
         end;
     end;

end;

procedure TParticleHandler.ClearParticles;
begin
     Particles.Clear;
end;

procedure TParticleHandler.AddParticle(particle: TParticle);
begin
     Particles.Add(particle);
end;

function TParticleHandler.AddEffect(Definition: TEffectDefinition; X, Y, Z: Single): TEffect;
var
   Effect: TEffect;
begin
     Effect := TEffect.Create(Definition);
     Effect.X := X;
     Effect.Y := Y;
     Effect.Z := Z;
     Effects.Add(Effect);
     result := Effect;
end;

procedure TParticleHandler.Render;
var
   Matrix: TMatrix;
   Vec, vec2, dvec: array[0..2] of TVertex;
   I: Integer;
begin
	glPushMatrix();
//	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, @matrix[0,0]);
	InvertMatrix(matrix);

	vec[0][0] := 0.0;
	vec[0][1] := 0.0;
	vec[0][2] := 0.0;
	vec[1][0] := 0.15;
	vec[1][1] := 0.0;
	vec[1][2] := 0.0;
	vec[2][0] := 0.0;
	vec[2][1] := 0.15;
	vec[2][2] := 0.0;

        For i := 0 to 2 do
		vec2[i] := VectorTransform(vec[i], matrix);
	for i := 0 to 2 do begin
		dvec[0][i] := vec2[1][i] - vec2[0][i];
		dvec[1][i] := vec2[2][i] - vec2[0][i];
	end;

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        glBlendFunc(GL_ONE, GL_ONE);
        glEnable(GL_BLEND);

	glBindTexture(GL_TEXTURE_2D, Texture);

	glBegin(GL_QUADS);
        For I := Particles.Count - 1 downto 0 do
            (Particles[I] as TParticle).Draw(dvec[0], dvec[1]);
{	std::list<cParticle *>::iterator iter;
	for (iter = particles.begin(); iter != particles.end(); iter ++)
		(*iter)->Draw(dvec[0], dvec[1]);}
	glEnd();

	glEnable(GL_CULL_FACE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
end;

procedure TParticleHandler.LoadTexture(APNG: TPNGObject);
var
  Data: PCardinalArray;
  x, y, w, h: Integer;
begin
  Assert (Assigned (APNG));
  w := APNG.Width; h := APNG.Width;
  if (w mod 32 <> 0) or (h mod 32 <> 0) or (h = 0) or (w = 0) then
      raise Exception.Create ('image extension must be a multiple of 32');

	if texture <> 0 then
		glDeleteTextures(1, @texture);

	glGenTextures( 1, @texture );
	glBindTexture( GL_TEXTURE_2D, texture );

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );


  GetMem (Data, w * h * 4);
  try
    FillChar(Data^,  w * h * 4, 0);
    For y := 0 to h - 1 do
      For x := 0 to w - 1 do
        Data^[Y * w + x] := Cardinal(APNG.Pixels[x, y]) or (Cardinal(APNG.AlphaScanLine[y][x]) shl 24);

  	gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA, GL_UNSIGNED_BYTE, Data );
    FTextureWidth := w;
    FTextureHeight := h;
  finally
      FreeMem (Data);
  end;
end;

procedure TParticleHandler.Start;
var
  Index: Integer;
begin
  For Index := 0 to Effects.Count - 1 do
      (Effects[Index] as TEffect).Start;

end;

function TParticleHandler.GetEffectCount: Integer;
begin
    result := Effects.Count;
end;

function TParticleHandler.GetParticleCount: Integer;
begin
    result := Particles.Count;
end;

end.
