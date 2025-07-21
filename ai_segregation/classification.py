import streamlit as st
from tensorflow.keras.models import load_model
from tensorflow.keras.preprocessing import image
import numpy as np

@st.cache_resource
def get_model():
    # Make sure to retrain/save your model with VALID layer names (no '/')
    return load_model('garbage_classifier_model.h5')

model = get_model()
classes = ['cardboard', 'glass', 'metal', 'paper', 'plastic', 'trash']

st.title("Waste Image Classifier")
st.write("Upload a waste image to classify it as cardboard, glass, metal, paper, plastic, or trash.")

uploaded_file = st.file_uploader("Choose an image...", type=["jpg", "jpeg", "png"])

if uploaded_file:
    # Preprocessing
    img = image.load_img(uploaded_file, target_size=(150, 150))
    img_array = image.img_to_array(img) / 255.0
    img_array = np.expand_dims(img_array, axis=0)
    
    pred = model.predict(img_array)
    pred_class = classes[np.argmax(pred)]
    confidence = np.max(pred) * 100

    st.image(img, caption="Uploaded Image", use_column_width=True)
    st.markdown(f"**Predicted Class:** {pred_class}")
    st.markdown(f"**Confidence:** {confidence:.2f}%")
else:
    st.info("Please upload an image file.")

