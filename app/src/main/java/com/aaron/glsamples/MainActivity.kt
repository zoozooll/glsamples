package com.aaron.glsamples

import android.content.Intent
import android.graphics.Bitmap
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.os.Environment.getExternalStoragePublicDirectory
import android.provider.MediaStore
import android.provider.MediaStore.ACTION_IMAGE_CAPTURE
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        id_hello_world.setOnClickListener {
            openc()
        }
    }

    private val TAKE_PICTURE = 1
    private var imageUri: Uri? = null
    private fun openc() {
        val intent = Intent(ACTION_IMAGE_CAPTURE)
        val photo = File(getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM), "Pic.jpg")
        intent.putExtra(
            MediaStore.EXTRA_OUTPUT,
            Uri.fromFile(photo)
        )
        imageUri = Uri.fromFile(photo)
        startActivityForResult(intent, TAKE_PICTURE)
    }

    public override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        when (requestCode) {
            TAKE_PICTURE -> if (resultCode == RESULT_OK) {
                val selectedImage = imageUri
                contentResolver.notifyChange(selectedImage, null)
                val cr = contentResolver
                val bitmap: Bitmap
                try {
                    bitmap = android.provider.MediaStore.Images.Media
                        .getBitmap(cr, selectedImage)


                } catch (e: Exception) {
                    Log.e("Camera", e.toString())
                }

            }
        }
    }
}
