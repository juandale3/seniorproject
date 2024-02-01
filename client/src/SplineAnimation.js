import React from 'react';
import Spline from '@splinetool/react-spline';

export default class SplineAnimation extends React.Component {

  render() {


    // Render the Spline scene when loading is false
    return (
      <div className="spline-container">
        <Spline scene="https://prod.spline.design/HDUu1pBGp-P5n6GX/scene.splinecode" />
      </div>
    );
  }
}
