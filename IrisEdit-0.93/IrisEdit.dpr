program IrisEdit;

uses
  Forms,
  form_Coords in 'form_Coords.pas' {CoordForm},
  form_Main in 'form_Main.pas' {MainForm},
  form_edit in 'form_edit.pas' {EditForm},
  form_Progress in 'form_Progress.pas' {ProgressForm},
  form_Scale in 'form_Scale.pas' {ScaleForm},
  form_Settings in 'form_Settings.pas' {SettingsForm},
  form_TextureManager in 'form_TextureManager.pas' {TextureManagerForm},
  DeCAL in 'DeCAL.pas',
  md5 in 'md5.pas',
  mwFixedRecSort in 'mwFixedRecSort.pas',
  Const3DS in '3DS\Const3DS.pas',
  File3DS in '3DS\File3DS.pas',
  Types3DS in '3DS\Types3DS.pas',
  Utils3DS in '3DS\Utils3DS.pas',
  Unit_ArtLoader in 'Unit_ArtLoader.pas',
  Unit_Compatibility in 'Unit_Compatibility.pas',
  Unit_LightNodes in 'Unit_LightNodes.pas',
  Unit_Matrix in 'Unit_Matrix.pas',
  Unit_ModelLoader in 'Unit_ModelLoader.pas',
  Unit_Models in 'Unit_Models.pas',
  Unit_OpenGL in 'Unit_OpenGL.pas',
  Unit_OpenGLGeometry in 'Unit_OpenGLGeometry.pas',
  Unit_OpenGLWrapper in 'Unit_OpenGLWrapper.pas',
  Unit_Settings in 'Unit_Settings.pas',
  Unit_TextureManager in 'Unit_TextureManager.pas',
  Unit_TileDataLoader in 'Unit_TileDataLoader.pas',
  Unit_ViewPort in 'Unit_ViewPort.pas',
  Unit_IntegerList in 'Unit_IntegerList.pas',
  Unit_Rasterer in 'Unit_Rasterer.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Iris - Modeleditor 0.93';
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TSettingsForm, SettingsForm);
  Application.CreateForm(TTextureManagerForm, TextureManagerForm);
  Application.Run;
end.
