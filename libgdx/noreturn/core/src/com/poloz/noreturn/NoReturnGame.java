package com.poloz.noreturn;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.audio.Music;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.poloz.noreturn.state.GameStateManager;
import com.poloz.noreturn.state.MenuState;

public class NoReturnGame extends ApplicationAdapter {
	private SpriteBatch batch;
	public static final int width = 800;
    public static final int heigth = 480;
    public static final String title = "No Return";
    private Music bkgMusic;
	private GameStateManager gsm;


	@Override
	public void create () {
        gsm = new GameStateManager();
		batch = new SpriteBatch();
        bkgMusic = Gdx.audio.newMusic(Gdx.files.internal("birds-01.mp3"));
        bkgMusic.setLooping(true);
        bkgMusic.setVolume(0.2f);
        bkgMusic.play();
        Gdx.gl.glClearColor(0, 58, 2, 1);
        gsm.push(new MenuState(gsm));
	}

	@Override
	public void render () {
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
        gsm.update(Gdx.graphics.getDeltaTime());
        gsm.render(batch);
	}
	
	@Override
	public void dispose () {
		batch.dispose();
        bkgMusic.dispose();
	}
}
