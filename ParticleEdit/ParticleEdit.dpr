program ParticleEdit;

uses
  Forms,
  Form_ParticleEdit in 'Form_ParticleEdit.Pas' {ParticleEditForm},
  Unit_Particles in 'Unit_Particles.pas',
  Form_Textures in 'Form_Textures.pas' {ParticleTextureForm},
  pngimage in 'pngimage.pas',
  pnglang in 'pnglang.pas',
  pngzlib in 'pngzlib.pas',
  Unit_Matrix in 'Unit_Matrix.pas',
  LibXmlComps in 'LibXmlComps.pas',
  LibXmlParser in 'LibXmlParser.pas',
  Unit_XmlNode in 'Unit_XmlNode.pas',
  mwFixedRecSort in 'mwFixedRecSort.pas',
  DeCAL in 'DeCAL.pas',
  Form_EditTranslation in 'Form_EditTranslation.pas' {TranslationEditForm},
  form_Main in 'form_Main.pas' {MainForm},
  Unit_Settings in 'Unit_Settings.pas',
  Form_EditRotation in 'Form_EditRotation.pas' {RotationEditForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Iris - Particle Editor';
  Application.CreateForm(TMainForm, MainForm);
  Application.CreateForm(TRotationEditForm, RotationEditForm);
  Application.Run;
end.
