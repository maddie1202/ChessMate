package com.example.chessiegame;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.HttpMethod;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;
import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.material.snackbar.Snackbar;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FacebookAuthProvider;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;

import com.google.android.gms.tasks.Task;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.util.Base64;
import android.util.Log;
import com.google.android.gms.common.api.ApiException;
import com.google.firebase.auth.GoogleAuthProvider;

import android.os.Bundle;
import android.widget.Button;
import android.widget.Toast;
import android.view.View;
import android.content.Intent;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.FileInputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {


    Button googleLogin, facebookLogin;
    private FirebaseAuth mAuth;
    private FirebaseUser user;
    private GoogleSignInClient mGoogleSignInClient;
    private final int RC_SIGN_IN = 120;
    private CallbackManager callbackManager;
    private LoginManager loginManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        facebookLogin = findViewById(R.id.button_login1);
        googleLogin = findViewById(R.id.button_login2);

        GoogleSignInOptions gso = new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                //.requestIdToken(getString(R.string.default_web_client_id))
                .requestIdToken("687858811033-bnu1iink9mre00vd4ku4ds0avir67ig2.apps.googleusercontent.com")
                .requestEmail()
                .build();

        mGoogleSignInClient = GoogleSignIn.getClient(this, gso);
        mAuth = FirebaseAuth.getInstance();
        user = mAuth.getCurrentUser();

        googleLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                signIn(user);
            }
        });

        findViewById(R.id.imageView).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent( v.getContext(), HomeActivity.class);
                startActivity( intent );
            }
        });

        FacebookSdk.sdkInitialize(MainActivity.this);
        callbackManager = CallbackManager.Factory.create();
        facebookLogin();

        facebookLogin.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                /*
                Intent intent = new Intent( v.getContext(), HomeActivity.class);
                startActivity( intent );

                 */
                loginManager.logInWithReadPermissions(
                        MainActivity.this,
                        Arrays.asList(
                                "email",
                                "public_profile"));
            }
        });



        //printHashKey(getApplicationContext());
    }

    public void facebookLogin()
    {

        loginManager
                = LoginManager.getInstance();
        callbackManager
                = CallbackManager.Factory.create();

        loginManager
                .registerCallback(
                        callbackManager,
                        new FacebookCallback<LoginResult>() {

                            @Override
                            public void onSuccess(LoginResult loginResult)
                            {

                                Log.d("Log IN", "signInWithCredential:success");
                                handleFacebookAccessToken(loginResult.getAccessToken());
                            }

                            @Override
                            public void onCancel()
                            {
                                Log.v("LoginScreen", "---onCancel");
                            }

                            @Override
                            public void onError(FacebookException error)
                            {
                                // here write code when get error
                                Log.v("LoginScreen", "----onError: "
                                        + error.getMessage());
                            }
                        });
    }

    private void handleFacebookAccessToken(AccessToken token) {
        Log.d("tag", "handleFacebookAccessToken:" + token);

        AuthCredential credential = FacebookAuthProvider.getCredential(token.getToken());
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            user = mAuth.getCurrentUser();
                            registerNewUser(user.getUid(), user.getEmail());
                            Intent intent = new Intent(getApplicationContext(), HomeActivity.class);
                            startActivity( intent );
                            finish();
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("Sign In Ac", "signInWithCredential:failure", task.getException());
                            Toast.makeText(MainActivity.this, "Authentication failed.",
                                    Toast.LENGTH_SHORT).show();

                        }
                    }
                });
    }



    /*
    public void disconnectFromFacebook()
    {
        if (AccessToken.getCurrentAccessToken() == null) {
            return; // already logged out
        }

        new GraphRequest(
                AccessToken.getCurrentAccessToken(),
                "/me/permissions/",
                null,
                HttpMethod.DELETE,
                new GraphRequest
                        .Callback() {
                    @Override
                    public void onCompleted(GraphResponse graphResponse)
                    {
                        LoginManager.getInstance().logOut();
                    }
                })
                .executeAsync();
    }




    public void printHashKey(Context pContext)
    {

        // Add code to print out the key hash
        try {

            PackageInfo info = pContext.getPackageManager().getPackageInfo(
                    pContext.getPackageName(), PackageManager.GET_SIGNATURES);

            for (Signature signature : info.signatures) {

                MessageDigest md
                        = MessageDigest.getInstance("SHA");
                md.update(signature.toByteArray());
                Log.d("KeyHash:",
                        Base64.encodeToString(
                                md.digest(),
                                Base64.DEFAULT));
                Log.d("PAckaage", pContext.getPackageName());
            }
        }


        catch (PackageManager.NameNotFoundException e) {
            Log.e("TAG","printhaskey", e);
        }

        catch (NoSuchAlgorithmException e) {
            Log.e("TAG","printhaskey", e);
        }
    }

     */

    // checks if user has signed in successfully
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        // Result returned from launching the Intent from GoogleSignInApi.getSignInIntent(...);
        if (requestCode == RC_SIGN_IN) {
            Task<GoogleSignInAccount> task = GoogleSignIn.getSignedInAccountFromIntent(data);
            Exception ex = task.getException();
            if (task.isSuccessful()) {
                try {
                    // Google Sign In was successful, authenticate with Firebase
                    GoogleSignInAccount account = task.getResult(ApiException.class);
                    Log.d("Signin successful", "firebaseAuthWithGoogle:" + account.getId());
                    firebaseAuthWithGoogle(account.getIdToken());
                } catch (ApiException e) {
                    // Google Sign In failed, update UI appropriately
                    Log.w("Signin failed", "Google sign in failed", e);
                }
            } else {
                Log.w("Signin failed", ex.toString());
            }
        }

        callbackManager.onActivityResult(
                requestCode,
                resultCode,
                data);
    }

    private void firebaseAuthWithGoogle(String idToken) {
        AuthCredential credential = GoogleAuthProvider.getCredential(idToken, null);
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, task -> {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d("Signin Activity", "signInWithCredential:success");
                            user = mAuth.getCurrentUser();
                            registerNewUser(user.getUid(), user.getEmail());
                            Intent intent = new Intent( this, HomeActivity.class);
                            startActivity( intent );
                            finish();
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("Signin Activity", "signInWithCredential:failure", task.getException());
                        }
                });
    }

    private void signIn(FirebaseUser user) {
        if (user != null) {
            Intent intent = new Intent( this, HomeActivity.class);
            startActivity( intent );
            finish();
        } else {
            Intent signInIntent = mGoogleSignInClient.getSignInIntent();
            startActivityForResult(signInIntent, RC_SIGN_IN);
        }
    }

    public void registerNewUser(String uid, String email) {
        RequestQueue queue = Volley.newRequestQueue(this);
        String url = "http://ec2-user@ec2-54-153-82-188.us-west-1.compute.amazonaws.com:3000/makeuser/";

        JSONObject postData = new JSONObject();
        try {
            postData.put("userID", uid);
            postData.put("name", email);

        } catch (JSONException e) {
            e.printStackTrace();
        }

        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, postData,
                new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                Log.d("Signin Screen", response.toString());
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                error.printStackTrace();
            }
        });

        queue.add(jsonObjectRequest);
    }

    @Override
    public void onBackPressed() {
        // super.onBackPressed();
        Toast.makeText(MainActivity.this,"There is no back action",Toast.LENGTH_LONG).show();
        return;
    }

}