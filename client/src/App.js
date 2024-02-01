import React from 'react';
import './App.css';
import Home from './Home';
import { TailSpin } from 'react-loader-spinner'; // Import the TailSpin component

export default class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      loading: true,
    };
  }

  componentDidMount() {
    // Simulate an async action with a 5-second delay
    setTimeout(() => {
      this.setState({ loading: false });
    }, 5000); // 5000 milliseconds (5 seconds)
  }

  render() {
    const { loading } = this.state;

    if (loading) {
      // Display the loading spinner for 5 seconds
      return (
        <div className="App">
          <TailSpin
            color="#00BFFF" // Set the color of the spinner
            height={100} // Set the height and width of the spinner
            width={100}
          />
        </div>
      );
    }

    return (
      <div className="App">
        <Home />
      </div>
    );
  }
}
