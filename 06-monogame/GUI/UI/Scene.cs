using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace GUI.UIElements;

public class Scene
{
    private static ContentManager? _content;
    public static ContentManager content
    {
        get { return _content;  }
        set
        {
            if (_content is not null)
                return;

            _content = value;
        }
    }

    private static GraphicsDeviceManager? _graphics;
    public static GraphicsDeviceManager graphics
    {
        get { return _graphics;  }
        set
        {
            if (_graphics is not null)
                return;
            
            _graphics = value;
        }
    }
    
    private static SpriteBatch? _spriteBatch;

    public static SpriteBatch spriteBatch
    {
        get { return _spriteBatch; }
        set
        {
            if (_spriteBatch is not null)
                return;

            _spriteBatch = value;
        }
    }
    
    public List<Drawable> drawables;

    public Scene()
    {
        drawables = new List<Drawable>();
    }
    
    public void Initialize()
    {
        foreach (Drawable drawable in drawables)
            drawable.Initialize();
    }

    public void Update(GameTime gameTime)
    {
        foreach (Drawable drawable in drawables)
            drawable.Update(gameTime);
    }

    public void Draw(GameTime gameTime)
    {
        spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend);
        
        foreach (Drawable drawable in drawables)
            drawable.Draw(gameTime);
        
        spriteBatch.End();
    }
}