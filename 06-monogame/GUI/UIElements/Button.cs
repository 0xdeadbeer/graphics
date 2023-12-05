using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace GUI.UIElements;

public class Button
{
    private Texture2D texture; 
    private Vector2 pos;
    private Vector2 size;
    private float rot;

    private Color _bgColor;
    public Color bgColor
    {
        get { return _bgColor; }
        set
        {
            _bgColor = value; 
            texture.SetData(new Color[] { _bgColor });
        }
    }

    public Button(Vector2? pos, Vector2? size, float? rot, GraphicsDeviceManager graphics)
    {
        this.pos = pos ?? Vector2.Zero;
        this.size = size ?? Vector2.One;
        this.rot = rot ?? 0.0f;

        texture = new Texture2D(graphics.GraphicsDevice, 1, 1);
        bgColor = Color.Green;
    }

    public void OnClick(GameTime gameTime)
    {
        Console.WriteLine("You pressed a button!");
        this.bgColor = Color.Yellow;
    }

    public void OnHover(GameTime gameTime)
    {
        Console.WriteLine("You're hovering over the button!");
        this.bgColor = Color.Red;
    }

    public void NoEvent(GameTime gameTime)
    {
        this.bgColor = Color.Green;
    }

    public void Update(GameTime gameTime)
    {
        var mstate = Mouse.GetState();
        Vector2 mousePosition = mstate.Position.ToVector2();

        bool insideHorizontalBoundary = (mousePosition.X > pos.X) && (mousePosition.X < (pos.X + size.X));
        bool insideVerticalBoundary = (mousePosition.Y > pos.Y) && (mousePosition.Y < (pos.Y + size.Y));

        if (!insideHorizontalBoundary || !insideVerticalBoundary)
        {
            this.NoEvent(gameTime);
            return;
        }

        if (Mouse.GetState().LeftButton == ButtonState.Pressed)
        {
            this.OnClick(gameTime);
            return;
        }
        
        this.OnHover(gameTime);
    } 
    
    public void Draw(SpriteBatch spriteBatch, GameTime gameTime)
    {
        spriteBatch.Draw(texture, new Rectangle((int) pos.X, (int) pos.Y, (int) size.X, (int) size.Y), Color.White);
    }
}