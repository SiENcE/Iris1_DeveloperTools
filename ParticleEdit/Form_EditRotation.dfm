object RotationEditForm: TRotationEditForm
  Left = 318
  Top = 135
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Edit Rotation'
  ClientHeight = 68
  ClientWidth = 203
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 36
    Height = 13
    Caption = 'Angle1:'
  end
  object Label2: TLabel
    Left = 104
    Top = 16
    Width = 33
    Height = 13
    Caption = 'Angle2'
  end
  object Edit1: TEdit
    Left = 56
    Top = 12
    Width = 41
    Height = 21
    TabOrder = 0
    Text = '0.0'
  end
  object Edit2: TEdit
    Left = 152
    Top = 12
    Width = 41
    Height = 21
    TabOrder = 1
    Text = '0.0'
  end
  object Button1: TButton
    Left = 40
    Top = 40
    Width = 75
    Height = 21
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
  end
  object Button2: TButton
    Left = 120
    Top = 40
    Width = 75
    Height = 21
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
end
