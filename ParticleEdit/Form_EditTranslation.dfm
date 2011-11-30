object TranslationEditForm: TTranslationEditForm
  Left = 352
  Top = 200
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Edit Translation'
  ClientHeight = 116
  ClientWidth = 147
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 8
    Top = 16
    Width = 10
    Height = 14
    Caption = 'X:'
  end
  object Label2: TLabel
    Left = 72
    Top = 16
    Width = 13
    Height = 14
    Caption = '+/-'
  end
  object Label3: TLabel
    Left = 8
    Top = 40
    Width = 11
    Height = 14
    Caption = 'Y:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Y:'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 72
    Top = 40
    Width = 13
    Height = 14
    Caption = '+/-'
  end
  object Label5: TLabel
    Left = 8
    Top = 64
    Width = 10
    Height = 14
    Caption = 'Z:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Y:'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 72
    Top = 64
    Width = 13
    Height = 14
    Caption = '+/-'
  end
  object Edit1: TEdit
    Left = 24
    Top = 12
    Width = 41
    Height = 22
    TabOrder = 0
    Text = '0.0'
  end
  object Edit2: TEdit
    Left = 96
    Top = 12
    Width = 41
    Height = 22
    TabOrder = 1
    Text = '0.0'
  end
  object Button1: TButton
    Left = 8
    Top = 88
    Width = 57
    Height = 21
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 6
  end
  object Button2: TButton
    Left = 72
    Top = 88
    Width = 65
    Height = 21
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 7
  end
  object Edit3: TEdit
    Left = 24
    Top = 36
    Width = 41
    Height = 22
    TabOrder = 2
    Text = '0.0'
  end
  object Edit4: TEdit
    Left = 96
    Top = 36
    Width = 41
    Height = 22
    TabOrder = 3
    Text = '0.0'
  end
  object Edit5: TEdit
    Left = 24
    Top = 60
    Width = 41
    Height = 22
    TabOrder = 4
    Text = '0.0'
  end
  object Edit6: TEdit
    Left = 96
    Top = 60
    Width = 41
    Height = 22
    TabOrder = 5
    Text = '0.0'
  end
end
