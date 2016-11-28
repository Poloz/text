package com.poloz.noreturn;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.audio.Music;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;

public class NoReturnGame extends ApplicationAdapter {
	SpriteBatch batch;
	Texture img;
	Texture img2;
	public static final int width = 800;
    public static final int heigth = 480;
    public static final String title = "No Return";

    private Music bkgMusic;


	@Override
	public void create () {
		batch = new SpriteBatch();
		img = new Texture("badlogic.jpg");
		img2 = new Texture("pine-001_texture.jpg");
        bkgMusic = Gdx.audio.newMusic(Gdx.files.internal("Zelentsov_Daleko.mp3"));
        bkgMusic.setLooping(true);
        bkgMusic.setVolume(0.2f);
        bkgMusic.play();
	}

	@Override
	public void render () {
		Gdx.gl.glClearColor(0, 58, 2, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		batch.begin();
		batch.draw(img, 450, 0);
		batch.draw(img2, 0, 0);
		batch.end();
	}
	
	@Override
	public void dispose () {
		batch.dispose();
		img.dispose();
	}
}
