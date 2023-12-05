using System;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Vector2 = Microsoft.Xna.Framework.Vector2;

namespace Introduction;

public class Character
{
    public Texture2D texture;
    public Texture2D noise; 
    public Vector2 pos;
    public Vector2 scale;
    public Vector2 rot;
    public float angles;
    public float speed;

    private bool glitchEnabled = false;
    private Effect glitch;
    private float time;

    public void Initialize()
    {
        speed = 300.0f;
        pos = Vector2.Zero;
        scale = new Vector2(1f);
        rot = Vector2.UnitX;
        angles = 0.0f;
    }

    public void LoadContent(ContentManager content)
    {
        texture = content.Load<Texture2D>("player_texture");
        noise = content.Load<Texture2D>("noise");
        glitch = content.Load<Effect>("glitch");
 
        if (glitch.Parameters["NoiseTexture"] is not null)
            glitch.Parameters["NoiseTexture"].SetValue(texture);
        
        if (glitch.Parameters["textureSize"] is not null)
            glitch.Parameters["textureSize"].SetValue(new Vector2(texture.Width, texture.Height));
    }

    public void Update(GameTime gameTime)
    {
        var kstate = Keyboard.GetState();
        if (kstate.IsKeyDown(Keys.W))
        {
            pos.Y -= speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
        }

        if (kstate.IsKeyDown(Keys.S))
        {
            pos.Y += speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
        }

        if (kstate.IsKeyDown(Keys.A))
        {
            pos.X -= speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
        }

        if (kstate.IsKeyDown(Keys.D))
        {
            pos.X += speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
        }

        if (kstate.IsKeyDown(Keys.K))
        {
            glitchEnabled = !glitchEnabled;
        }

        var mstate = Mouse.GetState();
        Vector2 mousePosition = new Vector2(mstate.X, mstate.Y);
        Vector2 distancePosition = mousePosition - pos;

        angles = (float) (Math.Atan2(distancePosition.Y, distancePosition.X));

        time += (float) gameTime.ElapsedGameTime.TotalSeconds;
        
        if (glitch.Parameters["Time"] is not null) 
            glitch.Parameters["Time"].SetValue(time);
    }

    public void Draw(GameTime gameTime, SpriteBatch spriteBatch)
    {
        spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend);
        Vector2 origin = new Vector2(texture.Width / 2, texture.Height / 2);

        if (glitchEnabled)
            glitch.CurrentTechnique.Passes[0].Apply();
        else
            time = 0; 
        
        spriteBatch.Draw(texture, pos, null, Color.White, angles, origin, scale, SpriteEffects.None, 0.0f);
        spriteBatch.End();
    } 
}