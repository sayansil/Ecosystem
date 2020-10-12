import unittest
import warnings
import json

from main import app

class BasicTests(unittest.TestCase):

    ######################
    # setup and teardown #
    ######################

    valid_demo_key = "111"
    invalid_demo_key = "000"


    # executed prior to each test
    def setUp(self):
        warnings.filterwarnings(
            action="ignore",
            message="unclosed",
            category=ResourceWarning)
        app.config['TESTING'] = True
        app.config['DEBUG'] = False
        self.app = app.test_client()
        self.assertFalse(app.debug)

    # executed after each test
    def tearDown(self):
        pass

    #########
    # tests #
    #########

    def test_sanity(self):
        response = self.app.get('/', follow_redirects=True)
        self.assertEqual(response.status_code, 200)

    def test_menu_page(self):
        response = self.app.get('/help/menu', follow_redirects=True)
        self.assertEqual(response.status_code, 200)

    def test_schema_page(self):
        response = self.app.get('/help/schema', follow_redirects=True)
        self.assertEqual(response.status_code, 200)

    ##############
    # test query #
    ##############

    def test_query_valid(self):
        response = self.app.post('/query', data=dict(
            api_key=self.valid_demo_key,
            years=20,
            silent='True'
        ), follow_redirects=True)
        self.assertEqual(response.status_code, 200)
        response_dict = response.json
        self.assertEqual(int(response_dict['status']), 0)

    def test_query_invalid(self):
        response = self.app.post('/query', data=dict(
            api_key=self.invalid_demo_key,
            years=20,
            silent='True'
        ), follow_redirects=True)
        self.assertEqual(response.status_code, 200)
        response_dict = response.json
        self.assertNotEqual(int(response_dict['status']), 0)

if __name__ == "__main__":
    unittest.main()

