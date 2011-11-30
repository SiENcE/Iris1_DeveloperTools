object ScaleForm: TScaleForm
  Left = 205
  Top = 171
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Scale Model'
  ClientHeight = 54
  ClientWidth = 357
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 10
    Height = 13
    Caption = 'X:'
  end
  object Label2: TLabel
    Left = 96
    Top = 8
    Width = 10
    Height = 13
    Caption = 'Y:'
  end
  object Label3: TLabel
    Left = 184
    Top = 8
    Width = 10
    Height = 13
    Caption = 'Z:'
  end
  object Edit1: TEdit
    Left = 32
    Top = 4
    Width = 57
    Height = 21
    TabOrder = 0
    Text = '1.00'
    OnChange = Edit1Change
    OnKeyDown = Edit1KeyDown
  end
  object Edit2: TEdit
    Left = 120
    Top = 4
    Width = 57
    Height = 21
    Enabled = False
    TabOrder = 1
    Text = '1.00'
  end
  object Edit3: TEdit
    Left = 208
    Top = 4
    Width = 57
    Height = 21
    Enabled = False
    TabOrder = 2
    Text = '1.00'
  end
  object Button2: TButton
    Left = 120
    Top = 28
    Width = 113
    Height = 21
    Caption = 'Scale'
    ModalResult = 1
    TabOrder = 3
  end
  object Button3: TButton
    Left = 240
    Top = 28
    Width = 113
    Height = 21
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 4
  end
  object CheckBox1: TCheckBox
    Left = 272
    Top = 6
    Width = 81
    Height = 17
    Caption = 'same value'
    Checked = True
    State = cbChecked
    TabOrder = 5
    OnClick = CheckBox1Click
  end
end
