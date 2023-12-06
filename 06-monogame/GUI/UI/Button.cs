using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace GUI.UIElements;

public class ButtonData
{
    public Color bgColor;
    public Color fgColor;

    public Color hoverBgColor;
    public Color hoverFgColor;

    public Color pressedBgColor;
    public Color pressedFgColor;
    
    public bool isPressed = false;
    public bool isReleased = false;
    public bool isHovered = false;
    public bool isNone = false;

    public Action<Button>? onClick;

    public ButtonData(Action<Button>? onClick)
    {
        this.onClick = onClick;
    }
}

public class Button : Drawable
{
    private DrawableData _drawableData;
    public ButtonData _buttonData; 
    
    public Button(DrawableData data, ButtonData buttonData)
    {
        _drawableData = data;
        _buttonData = buttonData;
    }

    public override void Initialize(GameTime gameTime)
    {
        
    }

    void onPressed()
    {
        if (_buttonData.isPressed)
            return;

        Console.WriteLine("I'm being pressed!");
        _buttonData.bgColor = Color.Red;

        _buttonData.onClick(this);
        
        _buttonData.isReleased = false;
        _buttonData.isPressed = true;
    }

    void onReleased()
    {
        if (!_buttonData.isPressed)
            return;

        _buttonData.bgColor = Color.Yellow; 
        
        _buttonData.isPressed = false;
        _buttonData.isReleased = true;
    }

    void onHovered()
    {
        if (_buttonData.isHovered)
            return;
        
        Console.WriteLine("I'm being hovered!");
        _buttonData.bgColor = Color.Yellow;

        _buttonData.isHovered = true;
    }

    void onNone()
    {
        _buttonData.bgColor = Color.Red;
        
        _buttonData.isPressed = false;
        _buttonData.isReleased = false;
        _buttonData.isHovered = false;
    }

    public override void Update(GameTime gameTime)
    {
        var mstate = Mouse.GetState();
        Vector2 mousePosition = mstate.Position.ToVector2();

        bool outsideHorizontalBoundary = (mousePosition.X < _drawableData.position.X) || (mousePosition.X > (_drawableData.position.X + _drawableData.scale.X));
        bool outsideVerticalBoundary = (mousePosition.Y < _drawableData.position.Y) || (mousePosition.Y > (_drawableData.position.Y + _drawableData.scale.Y));

        if (outsideHorizontalBoundary || outsideVerticalBoundary)
        {
            onNone();
            return;
        }

        if (mstate.LeftButton == ButtonState.Pressed)
        {
            onPressed();
            return;
        }

        if (mstate.LeftButton == ButtonState.Released)
        {
            onReleased();
        }

        onHovered();
    } 
    
    public override void Draw(GameTime gameTime)
    {
        Scene.spriteBatch.Draw(
            _drawableData.texture, 
            _drawableData.position, 
            null, 
            _buttonData.bgColor, 
            0.0f, 
            Vector2.Zero,
            _drawableData.scale, 
            SpriteEffects.None, 
            0f
        );
    }
}